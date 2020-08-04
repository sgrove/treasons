module Utils = {
  /* A helper for pretty-stringifying results */
  [@bs.val]
  external prettyStringify:
    ('a, [@bs.as {json|null|json}] _, [@bs.as {json|2|json}] _) => string =
    "JSON.stringify";

  /* Using some raw JavaScript to assign a value to window for console debugging */
  let assignToWindowForDeveloperDebug =
      (~name as _name: string, _value: 'b): unit =>
    switch ([%external window]) {
    | Some(_) =>
      %bs.raw
      {|window[_name] = _value|}
    | None => ()
    };

  let askForAuthAndThen =
      (auth: option(OneGraphAuth.t), serviceName, andThen: unit => unit) => {
    auth
    ->Belt.Option.map(auth =>
        OneGraphAuth.login(auth, serviceName)
        ->Js.Promise.then_(
            () => OneGraphAuth.isLoggedIn(auth, serviceName),
            _,
          )
        ->Js.Promise.then_(
            loginSuccess =>
              switch (loginSuccess) {
              | false =>
                Js.log2("The user did not grant auth to ", serviceName)
                ->Js.Promise.resolve
              | true =>
                Js.log2("Successfully logged into ", serviceName);
                andThen()->Js.Promise.resolve;
              },
            _,
          )
      )
    ->ignore;
  };
};

module RelayEnv = {
  /* Determine if we're rendering in a non-browser environment (e.g. node) */
  let isSsr: bool = [%bs.raw {|typeof window === "undefined"|}];

  let appId = "9694c378-e49a-4f07-abaf-6c742c03a63a";

  let oneGraphUrl = {j|https://serve.onegraph.com/graphql?app_id=$appId|j};

  let authConfig = OneGraphAuth.createOptions(~appId, ());

  let auth = isSsr ? None : Some(OneGraphAuth.create(authConfig));

  /* This is just a custom exception to indicate that something went wrong. */
  exception Graphql_error(string);

  /**
   * A standard fetch that sends our operation and variables to the
   * GraphQL server, and then decodes and returns the response.
   */
  let fetchQuery: ReasonRelay.Network.fetchFunctionPromise =
    (operation, variables, _cacheConfig) => {
      let authHeader =
        auth->Belt.Option.flatMap(auth => {
          OneGraphAuth.authHeaders_(auth)->Js.Dict.get("Authorization")
        });

      Fetch.(
        fetchWithInit(
          oneGraphUrl,
          RequestInit.make(
            ~method_=Post,
            ~body=
              Js.Dict.fromList([
                ("query", Js.Json.string(operation.text)),
                ("variables", variables),
              ])
              |> Js.Json.object_
              |> Js.Json.stringify
              |> BodyInit.make,
            ~headers=
              HeadersInit.make({
                "content-type": "application/json",
                "accept": "application/json",
                "Authorization": authHeader->Belt.Option.getWithDefault(""),
              }),
            (),
          ),
        )
        |> Js.Promise.then_(resp =>
             if (Response.ok(resp)) {
               Response.json(resp);
             } else {
               Js.Promise.reject(
                 Graphql_error(
                   "Request failed: " ++ Response.statusText(resp),
                 ),
               );
             }
           )
      );
    };

  module OneGraphSubscriptionClient = {
    type t;

    type subscriptionClientOptions = {oneGraphAuth: OneGraphAuth.t};

    [@bs.module "onegraph-subscription-client"] [@bs.new "SubscriptionClient"]
    external make: (string, subscriptionClientOptions) => t =
      "SubscriptionClient";

    type observable;
    [@bs.send] external request: (t, Js.t('a)) => observable = "request";
    [@bs.send]
    external subscribe: (observable, Js.t('a)) => unit = "subscribe";
  };

  let oneGraphSubscriptionClient =
    auth->Belt.Option.map(oneGraphAuth =>
      OneGraphSubscriptionClient.make(appId, {oneGraphAuth: oneGraphAuth})
    );

  /* Subscriptions to OneGraph can also work server-side with some addition packages, but we just disable them in SSR for now. */

  let subscriptionFunction: option(ReasonRelay.Network.subscribeFn) =
    oneGraphSubscriptionClient->Belt.Option.map(
      (
        oneGraphSubscriptionClient,
        operation: ReasonRelay.Network.operation,
        variables: Js.Json.t,
        _cacheConfig: ReasonRelay.cacheConfig,
      ) => {
      let subscribeObservable =
        oneGraphSubscriptionClient->OneGraphSubscriptionClient.request({
          "query": operation.text,
          "variables": variables,
          "operationName": operation.name,
        });

      let observable =
        ReasonRelay.Observable.make(sink => {
          subscribeObservable->OneGraphSubscriptionClient.subscribe({
            "next": data => {
              sink.next(data);
            },
            "error": sink.error,
            "complete": sink.complete,
          });
          None;
        });

      observable;
    });
  let network =
    ReasonRelay.Network.makePromiseBased(
      ~fetchFunction=fetchQuery,
      ~subscriptionFunction?,
      (),
    );

  let environment =
    ReasonRelay.Environment.make(
      ~network,
      ~store=
        ReasonRelay.Store.make(~source=ReasonRelay.RecordSource.make(), ()),
      (),
    );
};

module ErrorBoundary = {
  module Component = {
    [@bs.module "react-basic-error-boundary"] [@react.component]
    external make:
      (
        ~children: React.element,
        ~fallback: Js.Nullable.t(Js.Exn.t) => React.element,
        ~onError: Js.Nullable.t(Js.Exn.t) => unit
      ) =>
      React.element =
      "default";
  };

  [@react.component]
  let make = (~fallback, ~onError, ~children) =>
    <Component
      fallback={maybeError => fallback(maybeError |> Js.Nullable.toOption)}
      onError={maybeError => onError(maybeError |> Js.Nullable.toOption)}>
      children
    </Component>;
};

module GitHubIssueFragment = [%relay.fragment
  {|
  fragment Issues_GitHubIssueFragment on GitHubIssue {
    id
    number
    title
    state
  }
|}
];

module StreamingIssuesQuery = [%relay.query
  {|
  query Issues_StreamingIssuesQuery($repoName: String!, $repoOwner: String!) {
    gitHub {
      repository(name: $repoName, owner: $repoOwner) {
        issues(first: 50, orderBy: {field: CREATED_AT, direction: DESC}, labels: "streaming")
     @connection(key: "Issues_StreamingIssuesQuery_issues") {
          totalCount
          edges {
            node {
              ...Issues_GitHubIssueFragment
     id
     state
            }
          }
        }
      }
    }
  }
|}
];

module StreamingIssuesQueryComponent = {
  [@react.component]
  let make = (~repoName, ~repoOwner) => {
    let query =
      StreamingIssuesQuery.use(~variables={repoName, repoOwner}, ());

    let (closedStreamingIssueCount, totalStreamingIssueCount) =
      (
        switch (query) {
        | {
            gitHub:
              Some({repository: Some({issues: {totalCount} as issues})}),
          } =>
          let openIssueCount =
            StreamingIssuesQuery.getConnectionNodes(issues)
            ->Belt.Array.reduce(0, (acc, issue) =>
                switch (issue.state) {
                | `CLOSED => acc + 1
                | _ => acc
                }
              );
          Some((openIssueCount, totalCount));
        | _ => None
        }
      )
      ->Belt.Option.getWithDefault((0, 1));

    let progress =
      closedStreamingIssueCount->float_of_int
      /. totalStreamingIssueCount->float_of_int
      *. 100.0;

    Util.ReactStuff.(
      <>
        <div className="w-full">
          <div className="shadow w-full bg-grey-light mt-2">
            <div
              className="bg-teal-500 text-xs leading-none py-1 text-center text-white"
              style={ReactDOMRe.Style.make(
                ~width=progress->Js.Float.toString ++ "%",
                (),
              )}>
              {(progress->int_of_float->string_of_int ++ "%")->s}
            </div>
          </div>
        </div>
      </>
    );
  };
};

module StreamingIssuesSubscription = [%relay.subscription
  {|
  subscription Issues_StreamingIssuesSubscription($repoName: String!, $repoOwner: String!) {
    github {
      issuesEvent(input: {repoOwner: $repoOwner, repoName: $repoName}) {
        action
        issue {
          ...Issues_GitHubIssueFragment
        }
      }
    }
  }
|}
];

module StreamingIssuesSubscriptionComponent = {
  [@react.component]
  let make = (~repoName, ~repoOwner) => {
    let relayEnv = ReasonRelay.useEnvironmentFromContext();

    let (_subscriptionData, setSubscriptionData) = React.useState(() => None);
    let (_subscriptionEventCount, setSubscriptionEventCount) =
      React.useState(() => 0);

    React.useEffect0(() => {
      let disposable: ReasonRelay.Disposable.t =
        StreamingIssuesSubscription.subscribe(
          ~environment=relayEnv,
          ~variables={repoName, repoOwner},
          ~onNext=
            response => {
              setSubscriptionData(_ => Some(response));
              setSubscriptionEventCount(count => count + 1);
            },
          (),
        );

      /* Clean up/dispose of the subscription if we're unmounted */
      Some(() => disposable |> ReasonRelay.Disposable.dispose);
    });
    React.(<> null </>);
  };
};

module Container = {
  [@react.component]
  let make = () => {
    let repoName = "blog";
    let repoOwner = "sgrove";
    <React.Suspense
      fallback={<div> "One second, loading..."->React.string </div>}>
      <ErrorBoundary
        fallback={errors => {
          Js.log2("Fallback errors", errors);
          React.(<div> {string("Something went wrong")} </div>);
        }}
        onError={errors => Js.log2("Errors: ", errors)}>
        <ReasonRelay.Context.Provider environment=RelayEnv.environment>
          <StreamingIssuesQueryComponent repoName repoOwner />
          <hr />
          <StreamingIssuesSubscriptionComponent repoName repoOwner />
        </ReasonRelay.Context.Provider>
      </ErrorBoundary>
    </React.Suspense>;
  };
};

let default = Container.make;

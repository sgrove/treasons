/* @generated */

type enum_GitHubIssuesEventSubscriptionActionEnum =
  pri [>
    | `ASSIGNED
    | `CLOSED
    | `DELETED
    | `DEMILESTONED
    | `EDITED
    | `LABELED
    | `LOCKED
    | `MILESTONED
    | `OPENED
    | `PINNED
    | `REOPENED
    | `TRANSFERRED
    | `UNASSIGNED
    | `UNLABELED
    | `UNLOCKED
    | `UNPINNED
  ];

module Types = {
  [@ocaml.warning "-30"];
  type response_github = {issuesEvent: response_github_issuesEvent}
  and response_github_issuesEvent = {
    action: option(enum_GitHubIssuesEventSubscriptionActionEnum),
    issue: option(response_github_issuesEvent_issue),
  }
  and response_github_issuesEvent_issue = {
    fragmentRefs: ReasonRelay.fragmentRefs([ | `Issues_GitHubIssueFragment]),
  };

  type response = {github: response_github};
  type rawResponse = response;
  type variables = {
    repoName: string,
    repoOwner: string,
  };
};

module Internal = {
  type responseRaw;
  let responseConverter: Js.Dict.t(Js.Dict.t(Js.Dict.t(string))) = [%raw
    {json| {"__root":{"github_issuesEvent_action":{"n":""},"github_issuesEvent_issue":{"n":"","f":""}}} |json}
  ];
  let responseConverterMap = ();
  let convertResponse = v =>
    v
    ->ReasonRelay._convertObj(
        responseConverter,
        responseConverterMap,
        Js.undefined,
      );

  type rawResponseRaw = responseRaw;
  let convertRawResponse = convertResponse;

  let variablesConverter: Js.Dict.t(Js.Dict.t(Js.Dict.t(string))) = [%raw
    {json| {} |json}
  ];
  let variablesConverterMap = ();
  let convertVariables = v =>
    v
    ->ReasonRelay._convertObj(
        variablesConverter,
        variablesConverterMap,
        Js.undefined,
      );
};

module Utils = {
  external gitHubIssuesEventSubscriptionActionEnum_toString:
    enum_GitHubIssuesEventSubscriptionActionEnum => string =
    "%identity";
  open Types;
  let makeVariables = (~repoName, ~repoOwner): variables => {
    repoName,
    repoOwner,
  };
};

type operationType = ReasonRelay.subscriptionNode;

let node: operationType = [%raw
  {json| (function(){
var v0 = [
  {
    "defaultValue": null,
    "kind": "LocalArgument",
    "name": "repoName"
  },
  {
    "defaultValue": null,
    "kind": "LocalArgument",
    "name": "repoOwner"
  }
],
v1 = [
  {
    "fields": [
      {
        "kind": "Variable",
        "name": "repoName",
        "variableName": "repoName"
      },
      {
        "kind": "Variable",
        "name": "repoOwner",
        "variableName": "repoOwner"
      }
    ],
    "kind": "ObjectValue",
    "name": "input"
  }
],
v2 = {
  "alias": null,
  "args": null,
  "kind": "ScalarField",
  "name": "action",
  "storageKey": null
};
return {
  "fragment": {
    "argumentDefinitions": (v0/*: any*/),
    "kind": "Fragment",
    "metadata": null,
    "name": "Issues_StreamingIssuesSubscription",
    "selections": [
      {
        "alias": null,
        "args": null,
        "concreteType": "GithubSubscriptionRoot",
        "kind": "LinkedField",
        "name": "github",
        "plural": false,
        "selections": [
          {
            "alias": null,
            "args": (v1/*: any*/),
            "concreteType": "GitHubIssuesEventSubscriptionPayload",
            "kind": "LinkedField",
            "name": "issuesEvent",
            "plural": false,
            "selections": [
              (v2/*: any*/),
              {
                "alias": null,
                "args": null,
                "concreteType": "GitHubIssue",
                "kind": "LinkedField",
                "name": "issue",
                "plural": false,
                "selections": [
                  {
                    "args": null,
                    "kind": "FragmentSpread",
                    "name": "Issues_GitHubIssueFragment"
                  }
                ],
                "storageKey": null
              }
            ],
            "storageKey": null
          }
        ],
        "storageKey": null
      }
    ],
    "type": "Subscription",
    "abstractKey": null
  },
  "kind": "Request",
  "operation": {
    "argumentDefinitions": (v0/*: any*/),
    "kind": "Operation",
    "name": "Issues_StreamingIssuesSubscription",
    "selections": [
      {
        "alias": null,
        "args": null,
        "concreteType": "GithubSubscriptionRoot",
        "kind": "LinkedField",
        "name": "github",
        "plural": false,
        "selections": [
          {
            "alias": null,
            "args": (v1/*: any*/),
            "concreteType": "GitHubIssuesEventSubscriptionPayload",
            "kind": "LinkedField",
            "name": "issuesEvent",
            "plural": false,
            "selections": [
              (v2/*: any*/),
              {
                "alias": null,
                "args": null,
                "concreteType": "GitHubIssue",
                "kind": "LinkedField",
                "name": "issue",
                "plural": false,
                "selections": [
                  {
                    "alias": null,
                    "args": null,
                    "kind": "ScalarField",
                    "name": "id",
                    "storageKey": null
                  },
                  {
                    "alias": null,
                    "args": null,
                    "kind": "ScalarField",
                    "name": "number",
                    "storageKey": null
                  },
                  {
                    "alias": null,
                    "args": null,
                    "kind": "ScalarField",
                    "name": "title",
                    "storageKey": null
                  },
                  {
                    "alias": null,
                    "args": null,
                    "kind": "ScalarField",
                    "name": "state",
                    "storageKey": null
                  }
                ],
                "storageKey": null
              }
            ],
            "storageKey": null
          }
        ],
        "storageKey": null
      }
    ]
  },
  "params": {
    "cacheID": "45b9a37f61cc8f4e57dfe0711ed93bb6",
    "id": null,
    "metadata": {},
    "name": "Issues_StreamingIssuesSubscription",
    "operationKind": "subscription",
    "text": "subscription Issues_StreamingIssuesSubscription(\n  $repoName: String!\n  $repoOwner: String!\n) {\n  github {\n    issuesEvent(input: {repoOwner: $repoOwner, repoName: $repoName}) {\n      action\n      issue {\n        ...Issues_GitHubIssueFragment\n        id\n      }\n    }\n  }\n}\n\nfragment Issues_GitHubIssueFragment on GitHubIssue {\n  id\n  number\n  title\n  state\n}\n"
  }
};
})() |json}
];

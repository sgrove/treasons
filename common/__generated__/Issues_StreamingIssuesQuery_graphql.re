/* @generated */

type enum_GitHubIssueState = pri [> | `CLOSED | `OPEN];

module Types = {
  [@ocaml.warning "-30"];
  type response_gitHub = {repository: option(response_gitHub_repository)}
  and response_gitHub_repository = {issues: response_gitHub_repository_issues}
  and response_gitHub_repository_issues = {
    totalCount: int,
    edges: option(array(option(response_gitHub_repository_issues_edges))),
  }
  and response_gitHub_repository_issues_edges = {
    node: option(response_gitHub_repository_issues_edges_node),
  }
  and response_gitHub_repository_issues_edges_node = {
    id: string,
    state: enum_GitHubIssueState,
    fragmentRefs: ReasonRelay.fragmentRefs([ | `Issues_GitHubIssueFragment]),
  };

  type response = {gitHub: option(response_gitHub)};
  type rawResponse = response;
  type refetchVariables = {
    repoName: option(string),
    repoOwner: option(string),
  };
  let makeRefetchVariables = (~repoName=?, ~repoOwner=?, ()): refetchVariables => {
    repoName,
    repoOwner,
  };
  type variables = {
    repoName: string,
    repoOwner: string,
  };
};

module Internal = {
  type responseRaw;
  let responseConverter: Js.Dict.t(Js.Dict.t(Js.Dict.t(string))) = [%raw
    {json| {"__root":{"gitHub":{"n":""},"gitHub_repository":{"n":""},"gitHub_repository_issues_edges":{"n":"","na":""},"gitHub_repository_issues_edges_node":{"n":"","f":""}}} |json}
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

type queryRef;

module Utils = {
  external gitHubIssueState_toString: enum_GitHubIssueState => string =
    "%identity";
  open Types;
  let getConnectionNodes:
    response_gitHub_repository_issues =>
    array(response_gitHub_repository_issues_edges_node) =
    connection =>
      switch (connection.edges) {
      | None => [||]
      | Some(edges) =>
        edges
        ->Belt.Array.keepMap(edge =>
            switch (edge) {
            | None => None
            | Some(edge) =>
              switch (edge.node) {
              | None => None
              | Some(node) => Some(node)
              }
            }
          )
      };

  let makeVariables = (~repoName, ~repoOwner): variables => {
    repoName,
    repoOwner,
  };
};

type operationType = ReasonRelay.queryNode;

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
    "kind": "Variable",
    "name": "name",
    "variableName": "repoName"
  },
  {
    "kind": "Variable",
    "name": "owner",
    "variableName": "repoOwner"
  }
],
v2 = {
  "kind": "Literal",
  "name": "labels",
  "value": "streaming"
},
v3 = {
  "kind": "Literal",
  "name": "orderBy",
  "value": {
    "direction": "DESC",
    "field": "CREATED_AT"
  }
},
v4 = {
  "alias": null,
  "args": null,
  "kind": "ScalarField",
  "name": "totalCount",
  "storageKey": null
},
v5 = {
  "alias": null,
  "args": null,
  "kind": "ScalarField",
  "name": "id",
  "storageKey": null
},
v6 = {
  "alias": null,
  "args": null,
  "kind": "ScalarField",
  "name": "state",
  "storageKey": null
},
v7 = {
  "alias": null,
  "args": null,
  "kind": "ScalarField",
  "name": "__typename",
  "storageKey": null
},
v8 = {
  "alias": null,
  "args": null,
  "kind": "ScalarField",
  "name": "cursor",
  "storageKey": null
},
v9 = {
  "alias": null,
  "args": null,
  "concreteType": "GitHubPageInfo",
  "kind": "LinkedField",
  "name": "pageInfo",
  "plural": false,
  "selections": [
    {
      "alias": null,
      "args": null,
      "kind": "ScalarField",
      "name": "endCursor",
      "storageKey": null
    },
    {
      "alias": null,
      "args": null,
      "kind": "ScalarField",
      "name": "hasNextPage",
      "storageKey": null
    }
  ],
  "storageKey": null
},
v10 = [
  {
    "kind": "Literal",
    "name": "first",
    "value": 50
  },
  (v2/*: any*/),
  (v3/*: any*/)
];
return {
  "fragment": {
    "argumentDefinitions": (v0/*: any*/),
    "kind": "Fragment",
    "metadata": null,
    "name": "Issues_StreamingIssuesQuery",
    "selections": [
      {
        "alias": null,
        "args": null,
        "concreteType": "GitHubQuery",
        "kind": "LinkedField",
        "name": "gitHub",
        "plural": false,
        "selections": [
          {
            "alias": null,
            "args": (v1/*: any*/),
            "concreteType": "GitHubRepository",
            "kind": "LinkedField",
            "name": "repository",
            "plural": false,
            "selections": [
              {
                "alias": "issues",
                "args": [
                  (v2/*: any*/),
                  (v3/*: any*/)
                ],
                "concreteType": "GitHubIssueConnection",
                "kind": "LinkedField",
                "name": "__Issues_StreamingIssuesQuery_issues_connection",
                "plural": false,
                "selections": [
                  (v4/*: any*/),
                  {
                    "alias": null,
                    "args": null,
                    "concreteType": "GitHubIssueEdge",
                    "kind": "LinkedField",
                    "name": "edges",
                    "plural": true,
                    "selections": [
                      {
                        "alias": null,
                        "args": null,
                        "concreteType": "GitHubIssue",
                        "kind": "LinkedField",
                        "name": "node",
                        "plural": false,
                        "selections": [
                          (v5/*: any*/),
                          (v6/*: any*/),
                          (v7/*: any*/),
                          {
                            "args": null,
                            "kind": "FragmentSpread",
                            "name": "Issues_GitHubIssueFragment"
                          }
                        ],
                        "storageKey": null
                      },
                      (v8/*: any*/)
                    ],
                    "storageKey": null
                  },
                  (v9/*: any*/)
                ],
                "storageKey": "__Issues_StreamingIssuesQuery_issues_connection(labels:\"streaming\",orderBy:{\"direction\":\"DESC\",\"field\":\"CREATED_AT\"})"
              }
            ],
            "storageKey": null
          }
        ],
        "storageKey": null
      }
    ],
    "type": "Query",
    "abstractKey": null
  },
  "kind": "Request",
  "operation": {
    "argumentDefinitions": (v0/*: any*/),
    "kind": "Operation",
    "name": "Issues_StreamingIssuesQuery",
    "selections": [
      {
        "alias": null,
        "args": null,
        "concreteType": "GitHubQuery",
        "kind": "LinkedField",
        "name": "gitHub",
        "plural": false,
        "selections": [
          {
            "alias": null,
            "args": (v1/*: any*/),
            "concreteType": "GitHubRepository",
            "kind": "LinkedField",
            "name": "repository",
            "plural": false,
            "selections": [
              {
                "alias": null,
                "args": (v10/*: any*/),
                "concreteType": "GitHubIssueConnection",
                "kind": "LinkedField",
                "name": "issues",
                "plural": false,
                "selections": [
                  (v4/*: any*/),
                  {
                    "alias": null,
                    "args": null,
                    "concreteType": "GitHubIssueEdge",
                    "kind": "LinkedField",
                    "name": "edges",
                    "plural": true,
                    "selections": [
                      {
                        "alias": null,
                        "args": null,
                        "concreteType": "GitHubIssue",
                        "kind": "LinkedField",
                        "name": "node",
                        "plural": false,
                        "selections": [
                          (v5/*: any*/),
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
                          (v6/*: any*/),
                          (v7/*: any*/)
                        ],
                        "storageKey": null
                      },
                      (v8/*: any*/)
                    ],
                    "storageKey": null
                  },
                  (v9/*: any*/)
                ],
                "storageKey": "issues(first:50,labels:\"streaming\",orderBy:{\"direction\":\"DESC\",\"field\":\"CREATED_AT\"})"
              },
              {
                "alias": null,
                "args": (v10/*: any*/),
                "filters": [
                  "orderBy",
                  "labels"
                ],
                "handle": "connection",
                "key": "Issues_StreamingIssuesQuery_issues",
                "kind": "LinkedHandle",
                "name": "issues"
              },
              (v5/*: any*/)
            ],
            "storageKey": null
          }
        ],
        "storageKey": null
      }
    ]
  },
  "params": {
    "cacheID": "07cfa4eedb5de665e22a23f0d31feafd",
    "id": null,
    "metadata": {
      "connection": [
        {
          "count": null,
          "cursor": null,
          "direction": "forward",
          "path": [
            "gitHub",
            "repository",
            "issues"
          ]
        }
      ]
    },
    "name": "Issues_StreamingIssuesQuery",
    "operationKind": "query",
    "text": "query Issues_StreamingIssuesQuery(\n  $repoName: String!\n  $repoOwner: String!\n) {\n  gitHub {\n    repository(name: $repoName, owner: $repoOwner) {\n      issues(first: 50, orderBy: {field: CREATED_AT, direction: DESC}, labels: \"streaming\") {\n        totalCount\n        edges {\n          node {\n            ...Issues_GitHubIssueFragment\n            id\n            state\n            __typename\n          }\n          cursor\n        }\n        pageInfo {\n          endCursor\n          hasNextPage\n        }\n      }\n      id\n    }\n  }\n}\n\nfragment Issues_GitHubIssueFragment on GitHubIssue {\n  id\n  number\n  title\n  state\n}\n"
  }
};
})() |json}
];

include ReasonRelay.MakeLoadQuery({
  type variables = Types.variables;
  type loadedQueryRef = queryRef;
  type response = Types.response;
  let query = node;
  let convertVariables = Internal.convertVariables;
});

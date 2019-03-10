type agent = {id: string};

let convertAgent = agent => DataModel.toAgent(~id=agent.id);

type user = {
  id: string,
  name: string,
  auth0Id: option(string),
  agentId: option(string),
  agent: option(agent),
};

type t = option(user);

module Query = [%graphql
  {|
    query user ($auth0Id: String) {
        user:
          user(auth0Id: $auth0Id)  @bsRecord{
            id
            auth0Id
            name
            agentId
            agent: Agent  @bsRecord{
              id
            }
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let component =
    (
      auth0Id: option(string),
      innerComponentFn: 'a => ReasonReact.reactElement,
    ) =>
  switch (auth0Id) {
  | Some(auth) =>
    let query = Query.make(~auth0Id=auth, ());
    QueryComponent.make(
      ~variables=query##variables, ~pollInterval=5000, ({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(e => e##user)
      |> E.R.fmap(e => innerComponentFn(e))
      |> E.R.id
    )
    |> ReasonReact.element;
  | None => innerComponentFn(None)
  };

let withLoggedInUserQuery = (innerComponentFn: 'a => ReasonReact.reactElement) => {
  let isLoggedIn = Auth0.isLoggedIn();
  isLoggedIn ?
    component(Auth0.userId(), innerComponentFn) : innerComponentFn(None);
};
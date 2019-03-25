type channel = {
  id: string,
  name: string,
  isPublic: bool,
};

let toChannel = (ch: channel) =>
  DataModel.Channel.make(
    ~id=ch.id,
    ~name=ch.name,
    ~isArchived=false,
    ~isPublic=ch.isPublic,
    (),
  );

type agent = {
  id: string,
  channels: Js.Array.t(option(channel)),
};

let toAgent = (a: agent) =>
  DataModel.Agent.make(
    ~id=a.id,
    ~channels=a.channels |> E.A.O.concatSomes |> E.A.fmap(toChannel),
    (),
  );

type user = {
  id: string,
  name: string,
  auth0Id: option(string),
  agentId: option(string),
  agent: option(agent),
};

type t = option(user);

let toUser = (a: user) =>
  DataModel.User.make(
    ~id=a.id,
    ~auth0Id=a.auth0Id,
    ~agent=a.agent |> E.O.fmap(toAgent),
    (),
  );

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
              channels: Channels @bsRecord{
                name
                id
                isPublic
              }
            }
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

type innerType =
  ApolloUtils.QResponse.tri(DataModel.User.t) => ReasonReact.reactElement;

let component = (auth0Id: string, innerComponentFn: innerType) => {
  let query = Query.make(~auth0Id, ());
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult2
    |> ApolloUtils.QResponse.fmap(e => e##user |> E.O.fmap(toUser))
    |> ApolloUtils.QResponse.optionalToMissing
    |> innerComponentFn
  )
  |> E.React.el;
};

let withLoggedInUserQuery = (innerComponentFn: innerType) =>
  switch (Context.Auth.AuthTokens.make_from_storage()) {
  | None => innerComponentFn(Error("Not loaded"))
  | Some(tokens) =>
    Context.Auth.Actions.logoutIfTokenIsObsolete(tokens);
    switch (Context.Auth.AuthTokens.auth0Id(tokens)) {
    | Some(auth0Id) => component(auth0Id, innerComponentFn)
    | None => innerComponentFn(Error("No token"))
    };
  };

let withLoggedInUserQuery2 = innerComponentFn =>
  switch (Context.Auth.AuthTokens.make_from_storage()) {
  | None => innerComponentFn(Context.Me.WithoutTokens)
  | Some(tokens) =>
    Context.Auth.Actions.logoutIfTokenIsObsolete(tokens);
    switch (tokens |> Context.Auth.AuthTokens.auth0Id) {
    | None => innerComponentFn(Context.Me.WithoutTokens)
    | Some(auth0Id) =>
      let query = Query.make(~auth0Id, ());
      QueryComponent.make(~variables=query##variables, ({result}) =>
        result
        |> ApolloUtils.apolloResponseToResult2
        |> ApolloUtils.QResponse.fmap(e => e##user |> E.O.fmap(toUser))
        |> ApolloUtils.QResponse.optionalToMissing
        |> (
          e =>
            switch (e) {
            | Success(c) =>
              innerComponentFn(
                Context.Me.WithTokensAndUserData({
                  authTokens: tokens,
                  userData: c,
                }),
              )
            | _ =>
              innerComponentFn(
                Context.Me.WithTokensAndUserLoading({
                  authTokens: tokens,
                  loadingUserData: e,
                }),
              )
            }
        )
      )
      |> E.React.el;
    };
  };
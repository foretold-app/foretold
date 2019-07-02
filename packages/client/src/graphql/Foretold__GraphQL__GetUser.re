type channel = {
  id: string,
  name: string,
  isPublic: bool,
};

type channelMembership = {
  role: Types.channelMembershipRole,
  channel: option(channel),
};

type channelMemberships = Js.Array.t(option(channelMembership));

let toChannel = (ch: channel) =>
  Primary.Channel.make(
    ~id=ch.id,
    ~name=ch.name,
    ~isArchived=false,
    ~isPublic=ch.isPublic,
    (),
  );

let toChannelMembership = (ch: channelMembership): Types.channelMembership =>
  Primary.ChannelMembership.make(
    ~channel=ch.channel |> E.O.fmap(toChannel),
    ~role=`ADMIN,
    ~agent=None,
    (),
  );

type agent = {
  id: string,
  name: option(string),
  channelMemberships,
};

let toAgent = (a: agent) =>
  Primary.Agent.make(
    ~id=a.id,
    ~name=a.name,
    ~channelMemberships=
      a.channelMemberships
      |> E.A.O.concatSomes
      |> E.A.fmap(toChannelMembership)
      |> E.O.some,
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
  Primary.User.make(
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
            name
            id
            auth0Id
            agentId
            agent: Agent  @bsRecord{
              id
              name
              channelMemberships @bsRecord{
                role
                channel @bsRecord{
                  name
                  id
                  isPublic
                }
              }
            }
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let inner =
    (tokens: Context.Auth.Auth0Tokens.t, auth0Id: string, innerComponentFn) => {
  let query = Query.make(~auth0Id, ());
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> E.HttpResponse.fromApollo
    |> E.HttpResponse.fmap(e => e##user |> E.O.fmap(toUser))
    |> E.HttpResponse.optionalToMissing
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

let logOutIfTokensObsolete = t => {
  Context.Auth.Actions.logoutIfTokenIsObsolete(t);
  t;
};

let withLoggedInUserQuery = innerComponentFn =>
  Context.Auth.Auth0Tokens.make_from_storage()
  |> E.O.fmap(logOutIfTokensObsolete)
  |> E.O.bind(_, (tokens: Context.Auth.Auth0Tokens.t) =>
       tokens
       |> Context.Auth.Auth0Tokens.auth0Id
       |> E.O.fmap(auth0Id => (tokens, auth0Id))
     )
  |> E.O.fmap(((tokens, auth0Id)) =>
       Foretold__GraphQL__Authentication.component(
         tokens,
         inner(tokens, auth0Id, innerComponentFn),
       )
     )
  |> E.O.default(innerComponentFn(Context.Me.WithoutTokens));
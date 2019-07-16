type channel = {
  id: string,
  name: string,
  isPublic: bool,
};

type channelMembership = {
  role: Types.channelMembershipRole,
  channel: option(channel),
};

type preference = {
  id: string,
  stopAllEmails: option(bool),
};

type channelMemberships = Js.Array.t(option(channelMembership));

type agent = {
  id: string,
  name: option(string),
  channelMemberships,
  preference: option(preference),
};

type user = {
  id: string,
  name: string,
  email: option(string),
  picture: option(string),
  auth0Id: option(string),
  agentId: option(string),
  agent: option(agent),
};

type t = option(user);

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

let toPreference = (a: preference) =>
  Primary.Preference.make(~id=a.id, ~stopAllEmails=a.stopAllEmails, ());

let toAgent = (a: agent) =>
  Primary.Agent.make(
    ~id=a.id,
    ~name=a.name,
    ~channelMemberships=
      a.channelMemberships
      |> E.A.O.concatSomes
      |> E.A.fmap(toChannelMembership)
      |> E.O.some,
    ~preference=a.preference |> E.O.fmap(toPreference),
    (),
  );

let toUser = (a: user) =>
  Primary.User.make(
    ~id=a.id,
    ~auth0Id=a.auth0Id,
    ~email=a.email,
    ~picture=a.picture,
    ~agent=a.agent |> E.O.fmap(toAgent),
    (),
  );

module Query = [%graphql
  {|
    query user ($auth0Id: String) {
        user:
          user(auth0Id: $auth0Id)  @bsRecord{
            id
            name
            email
            picture
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
              preference: Preference @bsRecord{
                id
                stopAllEmails
              }
            }
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let inner = (tokens: Auth0Tokens.t, innerComponentFn) => {
  let query = Query.make();
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

let withLoggedInUserQuery = innerComponentFn => {
  let auth0tokens = Auth0Tokens.make_from_storage();

  auth0tokens |> E.O.fmap(Auth.Actions.logoutIfTokenIsObsolete) |> ignore;

  auth0tokens
  |> E.O.fmap((auth0tokens: Auth0Tokens.t) =>
       Foretold__GraphQL__Authentication.component(
         auth0tokens,
         inner(auth0tokens, innerComponentFn),
       )
     )
  |> E.O.default(innerComponentFn(Context.Me.WithoutTokens));
};
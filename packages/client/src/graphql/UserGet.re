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
  description: option(string),
  auth0Id: option(string),
  agentId: option(string),
  score: option(float),
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
    ~description=a.description,
    ~score=a.score,
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
            description
            auth0Id
            agentId
            score
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

let inner = innerComponentFn => {
  QueryComponent.make(({result}) =>
    result
    |> HttpResponse.fromApollo
    |> HttpResponse.fmap(e => e##user |> E.O.fmap(toUser))
    |> HttpResponse.optionalToMissing
    |> (
      e =>
        switch (e) {
        | Success(c) =>
          innerComponentFn(Me.WithTokensAndUserData({userData: c}))
        | _ =>
          innerComponentFn(Me.WithTokensAndUserLoading({loadingUserData: e}))
        }
    )
  )
  |> ReasonReact.element;
};
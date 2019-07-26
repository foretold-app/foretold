let toChannel = ch =>
  Primary.Channel.make(
    ~id=ch##id,
    ~name=ch##name,
    ~isArchived=false,
    ~isPublic=ch##isPublic,
    (),
  );

let toPreference = a =>
  Primary.Preference.make(~id=a##id, ~stopAllEmails=a##stopAllEmails, ());

let toAgent = a =>
  Primary.Agent.make(
    ~id=a##id,
    ~name=a##name,
    ~channelMemberships=None,
    ~preference=a##preference |> E.O.fmap(toPreference),
    (),
  );

let toBots = bots =>
  bots
  |> E.A.O.concatSome
  |> Array.map(bot => Primary.Bot.make(~id=bot##id, ()))
  |> E.O.some;

let toUser = a =>
  Primary.User.make(
    ~id=a##id,
    ~auth0Id=a##auth0Id,
    ~email=a##email,
    ~picture=a##picture,
    ~description=a##description,
    ~score=a##score,
    ~agent=a##agent |> E.O.fmap(toAgent),
    ~bots=a##bots |> toBots,
    (),
  );

module Query = [%graphql
  {|
    query user ($auth0Id: String) {
        user:
          user(auth0Id: $auth0Id) {
            id
            name
            email
            picture
            description
            auth0Id
            agentId
            score
            agent: Agent {
              id
              name
              preference: Preference {
                id
                stopAllEmails
              }
            }
            bots: Bots {
              id
            }
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let inner = innerComponentFn => {
  <QueryComponent>
    ...{({result}) =>
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
            innerComponentFn(
              Me.WithTokensAndUserLoading({loadingUserData: e}),
            )
          }
      )
    }
  </QueryComponent>;
};
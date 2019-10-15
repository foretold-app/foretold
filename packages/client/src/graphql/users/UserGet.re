let toChannel = ch =>
  Primary.Channel.make(
    ~id=ch##id,
    ~name=ch##name,
    ~isArchived=false,
    ~isPublic=ch##isPublic,
    (),
  );

let toPreference = a =>
  Primary.Preference.make(
    ~id=a##id,
    ~stopAllEmails=a##stopAllEmails,
    ~enableExperimentalFeatures=a##enableExperimentalFeatures,
    (),
  );

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
  |> Array.map(bot =>
       Primary.Bot.make(
         ~id=bot##id,
         ~name=Some(bot##name),
         ~agent=
           bot##agent
           |> E.O.fmap(agent =>
                Primary.Agent.make(~id=agent##id, ~name=agent##name, ())
              ),
         (),
       )
     )
  |> E.O.some;

let toUser = a =>
  Primary.User.make(
    ~id=a##id,
    ~auth0Id=a##auth0Id,
    ~agentId=a##agentId,
    ~email=a##email,
    ~picture=a##picture,
    ~description=a##description,
    ~agent=a##agent |> E.O.fmap(toAgent),
    ~bots=a##bots |> toBots,
    ~isEmailVerified=a##isEmailVerified,
    ~createdAt=Some(a##createdAt),
    ~name=a##name,
    (),
  );

module Query = [%graphql
  {|
    query user {
      authenticated {
          user {
            id
            name
            email
            picture
            description
            auth0Id
            agentId
            isEmailVerified
            createdAt @bsDecoder(fn: "E.J.toMoment")
            agent {
              id
              name
              preference {
                id
                stopAllEmails
                enableExperimentalFeatures
              }
            }
            bots {
              id
              name
              agent {
                id
                name
              }
            }
          }
      }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let inner = innerComponentFn => {
  <QueryComponent pollInterval={60 * 1000}>
    ...{({result}) =>
      result
      |> HttpResponse.fromApollo
      |> HttpResponse.fmap(e => e##authenticated##user |> E.O.fmap(toUser))
      |> HttpResponse.optionalToMissing
      |> (
        e =>
          switch (e) {
          | Success(user) => innerComponentFn(Some(user))
          | _ => innerComponentFn(None)
          }
      )
    }
  </QueryComponent>;
};

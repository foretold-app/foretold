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
    (),
  );

module Query = [%graphql
  {|
    query user ($auth0Id: String) {
      user(auth0Id: $auth0Id) {
        id
        name
        email
        picture
        description
        auth0Id
        agentId
        isEmailVerified
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
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let component = innerComponentFn => {
  <QueryComponent fetchPolicy="network-only">
    ...{({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(e => e##user |> E.O.fmap(toUser))
      |> E.R.fmap(innerComponentFn)
      |> E.R.id
    }
  </QueryComponent>;
};

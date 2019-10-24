[@bs.config {jsx: 3}];

let toAgent = agent => {
  let agentType = Primary.AgentType.getAgentType(~agent, ());

  Primary.Agent.make(
    ~id=agent##id,
    ~name=agent##name,
    ~measurementCount=Some(agent##measurementCount),
    ~agentType,
    (),
  );
};

module Query = [%graphql
  {|
    query getAgents (
        $excludeChannelId: String
        $types: [AgentType]
    ) {
      agents (
        excludeChannelId: $excludeChannelId
        types: $types
      ) {
        id
        name
        measurementCount
        user {
          id
          name
          description
          agentId
          picture
        }
        bot {
          id
          name
          description
          picture
          competitorType
          user {
              id
              name
              description
              picture
              agentId
          }
        }
      }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let component = (~excludeChannelId: string, ~types=?, innerFn) => {
  let query = Query.make(~excludeChannelId, ~types?, ());
  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> HttpResponse.fromApollo
      |> HttpResponse.fmap(e =>
           e##agents |> E.A.O.concatSome |> E.A.fmap(toAgent)
         )
      |> innerFn
    }
  </QueryComponent>;
};

let componentUsers = component(~types=[|Some(`USER)|]);

let toAgent = agent => {
  let agentType = Primary.AgentType.getAgentType(~agent, ());

  Primary.Agent.make(
    ~id=agent##id,
    ~name=agent##name,
    ~isMe=agent##isMe,
    ~agentType,
    (),
  );
};

module Query = [%graphql
  {|
    query getAgent ($id: String!) {
        agent(id: $id) {
            id
            isMe
            name
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

let component = (~id, innerFn) => {
  let notFound = "Agent not found" |> Utils.ste;
  let query = Query.make(~id, ());

  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> HttpResponse.fromApollo
      |> HttpResponse.fmap(e => e##agent |> E.O.fmap(toAgent))
      |> innerFn
    }
  </QueryComponent>;
};
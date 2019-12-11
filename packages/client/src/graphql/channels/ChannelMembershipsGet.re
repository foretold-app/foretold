module Query = [%graphql
  {|
    query getChannelMemberships($id: String!) {
      channel(id: $id){
        id
        channelMemberships{
            role
            permissions {
              mutations {
                allow
              }
            }
            agent {
              id
              name
              user {
                  id
                  name
                  description
                  picture
                  agentId
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
      }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let toChannelMemberships = (m): array(Types.channelMembership) => {
  m##channelMemberships
  |> E.A.O.concatSomes
  |> (
    e =>
      e
      |> E.A.fmap(r => {
           let agentType =
             E.O.bind(
               r##agent,
               Primary.AgentType.getAgentType(~agent=_, ()),
             );

           let agent =
             r##agent
             |> E.O.fmap(k =>
                  Primary.Agent.make(~id=k##id, ~agentType, ~name=k##name, ())
                );

           let allowMutations =
             r##permissions##mutations##allow
             |> E.A.O.concatSome
             |> E.A.to_list;

           let permissions = Primary.Permissions.make(allowMutations);

           Primary.ChannelMembership.make(
             ~role=r##role,
             ~agent,
             ~permissions=Some(permissions),
             (),
           );
         })
  );
};

let component = (~id, innerFn) => {
  let query = Query.make(~id, ());
  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> HttpResponse.fromApollo
      |> HttpResponse.fmap(e => e##channel |> E.O.fmap(toChannelMemberships))
      |> HttpResponse.optionalToMissing
      |> innerFn
    }
  </QueryComponent>;
};
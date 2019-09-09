module Query = [%graphql
  {|
    query getChannelMemberships($id: String!) {
      channelWithMemberships:
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
            }
        }
      }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let toChannelMemberships = (m): array(Types.channelMembership) => {
  let channelMemberships =
    m##channelMemberships
    |> E.A.O.concatSomes
    |> (
      e =>
        e
        |> E.A.fmap(r => {
             open Types;

             let agent =
               Primary.Agent.make(
                 ~id=r##agent |> E.O.fmap(r => r##id) |> E.O.default(""),
                 ~name=r##agent |> E.O.bind(_, r => r##name),
                 (),
               );

             let allowMutations =
               r##permissions##mutations##allow
               |> E.A.O.concatSome
               |> E.A.to_list;

             let permissions = Primary.Permissions.make(allowMutations);

             Primary.ChannelMembership.make(
               ~role=r##role,
               ~agent=Some(agent),
               ~permissions=Some(permissions),
               (),
             );
           })
    );
  channelMemberships;
};

let component = (~id, innerFn) => {
  let query = Query.make(~id, ());
  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> HttpResponse.fromApollo
      |> HttpResponse.fmap(e =>
           e##channelWithMemberships |> E.O.fmap(toChannelMemberships)
         )
      |> HttpResponse.optionalToMissing
      |> innerFn
    }
  </QueryComponent>;
};
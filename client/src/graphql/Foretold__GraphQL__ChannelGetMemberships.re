open Utils;

module Query = [%graphql
  {|
    query getChannel($id: String!) {
      channelWithMemberships:
      channel(id: $id){
        id
        channelMemberships{
            role
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

type innerType = {
  .
  "channelMemberships":
    Js.Array.t(
      option({
        .
        "agent":
          option({
            .
            "id": string,
            "name": option(string),
          }),
        "role": [ | `ADMIN | `VIEWER],
      }),
    ),
  "id": string,
};

let toChannelMemberships =
    (m: innerType): array(Context.Primary.Types.channelMembership) => {
  let channelMemberships =
    m##channelMemberships
    |> E.A.O.concatSomes
    |> (
      e =>
        e
        |> E.A.fmap(r => {
             open Context.Primary.Types;
             let agent =
               Context.Primary.Agent.make(
                 ~id=r##agent |> E.O.fmap(r => r##id) |> E.O.default(""),
                 ~name=r##agent |> E.O.bind(_, r => r##name),
                 (),
               );
             {channel: None, role: r##role, agent: Some(agent)};
           })
    );
  channelMemberships;
};

let component = (~id, innerFn) => {
  let query = Query.make(~id, ());
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> E.HttpResponse.fromApollo
    |> E.HttpResponse.fmap(e =>
         e##channelWithMemberships |> E.O.fmap(toChannelMemberships)
       )
    |> E.HttpResponse.optionalToMissing
    |> innerFn
  )
  |> E.React.el;
};
module Query = [%graphql
  {|
        mutation botUpdate($input: BotInput!) {
            botUpdate(input: $input) {
             id
            }
        }
    |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate =
    (mutation: Mutation.apolloMutation, name, description, competitorType) => {
  let m =
    Query.make(
      ~input={
        "name": name,
        "description": Some(description),
        "competitorType": competitorType,
      },
      (),
    );
  mutation(~variables=m##variables, ~refetchQueries=[|"user"|], ()) |> ignore;
};

let withMutation = innerComponentFn =>
  Mutation.make(~onError=e => Js.log2("Graphql Error:", e), innerComponentFn)
  |> E.React.el;
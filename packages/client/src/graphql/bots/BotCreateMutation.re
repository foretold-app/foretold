module Query = [%graphql
  {|
            mutation botCreate($input: BotInput!) {
                botCreate(input: $input) {
                 id
                }
            }
    |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate =
    (
      mutation: Mutation.apolloMutation,
      name,
      description,
      competitorType,
      picture,
    ) => {
  let m =
    Query.make(
      ~input={
        "name": name,
        "description": Some(description),
        "competitorType": competitorType,
        "picture": Some(picture),
      },
      (),
    );
  mutation(~variables=m##variables, ~refetchQueries=[|"user", "bots"|], ())
  |> ignore;
};

let withMutation = innerComponentFn =>
  <Mutation onError={e => Js.log2("Graphql Error:", e)}>
    ...innerComponentFn
  </Mutation>;

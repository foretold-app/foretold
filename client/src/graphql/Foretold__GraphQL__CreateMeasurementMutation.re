module Query = [%graphql
  {|
            mutation measurementCreate($input: MeasurableCreateInput!) {
                measurementCreate(input: $input) {
                  createdAt
                }
            }
    |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

type competitorType = [ | `COMPETITIVE | `OBJECTIVE];

let mutate =
    (
      mutation: Mutation.apolloMutation,
      measurableId: string,
      value: MeasurementValue.t,
      competitorType: competitorType,
      description: string,
    ) => {
  let m =
    Query.make(
      ~input={
        "measurableId": measurableId,
        "value": value |> MeasurementValue.encode,
        "description": description,
        "competitorType": competitorType,
      },
      (),
    );
  mutation(~variables=m##variables, ~refetchQueries=[|"getMeasurable"|], ())
  |> ignore;
};
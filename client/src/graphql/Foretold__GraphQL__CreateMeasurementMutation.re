module Query = [%graphql
  {|
            mutation measurementCreate($input: MeasurementCreateInput!) {
                measurementCreate(input: $input) {
                  createdAt
                }
            }
    |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

type competitorType = [ | `COMPETITIVE | `OBJECTIVE];

/* TODO: Finish this so that value returns the actually correct results. */
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
        "measurableId": Some(measurableId),
        "value": value |> MeasurementValue.encodeToGraphQLMutation,
        "description": Some(description),
        "competitorType": competitorType,
        "relevantAt": None,
        "taggedMeasurementId": None,
        "agentId": None,
      },
      (),
    );
  mutation(~variables=m##variables, ~refetchQueries=[|"getMeasurable"|], ())
  |> ignore;
};
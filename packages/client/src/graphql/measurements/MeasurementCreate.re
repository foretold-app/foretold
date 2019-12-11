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

let mutate =
    (
      mutation: Mutation.apolloMutation,
      measurableId: string,
      value: MeasurementValue.t,
      competitorType: Types.competitorType,
      description: string,
      valueText: string,
      agentId: string,
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
        "agentId": agentId == "" ? None : Some(agentId),
        "valueText": Some(valueText),
      },
      (),
    );

  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|"getMeasurements", "getMeasurables", "getMeasurable"|],
    (),
  )
  |> ignore;
};
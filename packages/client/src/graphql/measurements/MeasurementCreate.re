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

  // if "measurables" is added to the list of refreshQueries, it
  // causes a bug where once a user makes a prediction on anything
  // but the most recently-predicted distribution, the currently selected
  // measurable will change

  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|"measurements", "measurable"|],
    (),
  )
  |> ignore;
};
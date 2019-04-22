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
        "value":
          Some({
            "floatPoint": Some(3.0),
            "floatCdf":
              Some({
                "xs": [|Some(3.0), Some(5.0)|],
                "ys": [|Some(3.0)|],
              }),
          }),
        "description": Some(description),
        "competitorType": competitorType,
        "agentId": None,
      },
      (),
    );
  mutation(~variables=m##variables, ~refetchQueries=[|"getMeasurable"|], ())
  |> ignore;
};
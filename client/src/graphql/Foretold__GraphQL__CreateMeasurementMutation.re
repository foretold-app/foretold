module Query = [%graphql
  {|
            mutation measurementCreate($value: SequelizeJSON!, $description: String!, $competitorType:competitorType!, $measurableId:String!) {
                measurementCreate(value: $value, description: $description, competitorType: $competitorType, measurableId:$measurableId) {
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
      ~measurableId,
      ~value=value |> MeasurementValue.encode,
      ~description,
      ~competitorType,
      (),
    );
  mutation(~variables=m##variables, ~refetchQueries=[|"getMeasurable"|], ())
  |> ignore;
};
module Query = [%graphql
  {|
    mutation vote(
        $measurementId: MeasurementId!
        $input: VoteInput!
    ) {
        vote(
            measurementId: $measurementId
            input: $input
        ) {
         id
         voteAmount
        }
    }
  |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate = (mutation: Mutation.apolloMutation, measurementId, voteAmount) => {
  let input = {"voteAmount": voteAmount |> float_of_int |> E.J.fromNumber};
  let m =
    Query.make(~measurementId=E.J.fromString(measurementId), ~input, ());
  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|"getMeasurements"|],
    (),
  )
  |> ignore;
};
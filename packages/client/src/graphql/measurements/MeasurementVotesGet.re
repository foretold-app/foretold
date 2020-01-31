module Query = [%graphql
  {|
    query measurementVotes (
        $measurementId: MeasurementId
    ){
      votes (
        measurementId: $measurementId
      ){
        id
        voteAmount
        createdAt
        updatedAt
        agent {
            id
            name
            user {
                id
                name
                description
                picture
                agentId
            }
            bot {
                id
                name
                description
                picture
                competitorType
                user {
                    id
                    name
                    description
                    picture
                    agentId
                }
            }
        }
      }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let component = (~measurementId: option(string)=?, fn) => {
  let measurementId = measurementId |> E.O.fmap(id => id |> E.J.fromString);
  let query = Query.make(~measurementId?, ());

  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(e =>
           e##votes
           |> E.A.O.concatSomes
           |> E.A.fmap(Primary.Vote.convertJsObject2)
         )
      |> E.R.fmap(fn)
      |> E.R.id
    }
  </QueryComponent>;
};
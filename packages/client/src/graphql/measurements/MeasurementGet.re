module Query = [%graphql
  {|
    query measurement (
        $id: String!
     ) {
        measurement (
            id: $id
        ) {
          id
          createdAt @bsDecoder(fn: "E.J.toMoment")
          updatedAt @bsDecoder(fn: "E.J.toMoment")
          cancelledAt
          value {
              floatCdf {
                  xs
                  ys
              }
              floatPoint
              percentage
              binary
              unresolvableResolution
              comment
          }
          relevantAt @bsDecoder(fn: "E.J.O.toMoment")
          competitorType
          description
          valueText
          taggedMeasurementId
          totalVoteAmount
          vote {
              id
              voteAmount
              createdAt
              updatedAt
          }
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

          measurable {
            id
            name
            channelId
            expectedResolutionDate @bsDecoder(fn: "E.J.O.toMoment")
            state
            valueType
            stateUpdatedAt @bsDecoder(fn: "E.J.O.toMoment")
          }

          measurementScoreSet {
            primaryPointScore
            nonMarketLogScore
          }

          permissions {
            mutations {
              allow
            }
          }
       }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let component = (~id, innerFn) => {
  let query = Query.make(~id, ());
  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(e =>
           e##measurement
           |> Rationale.Option.fmap(MeasurementsGet.toMeasurement)
         )
      |> E.R.fmap(innerFn)
      |> E.R.id
    }
  </QueryComponent>;
};
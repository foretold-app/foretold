open Utils;

module Query = [%graphql
  {|
      query measurable ($id: String!) {
        measurable (id: $id){
          id
          name
          valueType
          channelId
          labelCustom
          resolutionEndpoint
          resolutionEndpointResponse
          labelSubject
          labelProperty
          iAmOwner
          state
          labelOnDate @bsDecoder(fn: "E.J.O.toMoment")
          stateUpdatedAt @bsDecoder(fn: "E.J.O.toMoment")
          expectedResolutionDate @bsDecoder(fn: "E.J.O.toMoment")
          createdAt @bsDecoder(fn: "E.J.toMoment")
          updatedAt @bsDecoder(fn: "E.J.toMoment")
          creator {
            id
            name
            user {
              id
              name
              description
              agentId
              picture
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
          series {
            id
            name
            channelId
          }
          min
          max
          previousAggregate {
              id
              valueText
              value {
                  floatCdf { xs ys }
                  floatPoint
                  percentage
                  binary
                  unresolvableResolution
                  comment
              }
          }
          recentMeasurement {
            id
            valueText
            value {
              floatCdf { xs ys }
              floatPoint
              percentage
              binary
              unresolvableResolution
              comment
            }
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

let toMeasurable = (m): Types.measurable => {
  let agent = m##creator |> MeasurablesGet.toAgent;

  let previousAggregate =
    m##previousAggregate
    |> E.O.fmap(measurement =>
         Primary.Measurement.make(
           ~id=measurement##id,
           ~valueText=measurement##valueText,
           ~value=measurement##value |> MeasurementValue.decodeGraphql,
           (),
         )
       );

  let recentMeasurement =
    m##recentMeasurement
    |> E.O.fmap(measurement =>
         Primary.Measurement.make(
           ~id=measurement##id,
           ~valueText=measurement##valueText,
           ~value=measurement##value |> MeasurementValue.decodeGraphql,
           (),
         )
       );

  let series =
    m##series
    |> E.O.fmap(r =>
         Primary.Series.make(
           ~id=r##id,
           ~name=r##name,
           ~channelId=r##channelId,
           (),
         )
       );

  let allowMutations =
    m##permissions##mutations##allow |> E.A.O.concatSome |> E.A.to_list;

  let permissions = Primary.Permissions.make(allowMutations);

  Primary.Measurable.make(
    ~id=m##id,
    ~name=m##name |> E.J.toString,
    ~channelId=m##channelId,
    ~labelCustom=m##labelCustom,
    ~resolutionEndpoint=m##resolutionEndpoint,
    ~resolutionEndpointResponse=m##resolutionEndpointResponse,
    ~createdAt=Some(m##createdAt),
    ~updatedAt=Some(m##updatedAt),
    ~expectedResolutionDate=m##expectedResolutionDate,
    ~state=Some(m##state |> Primary.MeasurableState.fromEnum),
    ~stateUpdatedAt=m##stateUpdatedAt,
    ~labelSubject=m##labelSubject,
    ~labelOnDate=m##labelOnDate,
    ~labelProperty=m##labelProperty,
    ~valueType=m##valueType,
    ~measurements=None,
    ~creator=agent,
    ~min=m##min,
    ~max=m##max,
    ~series,
    ~iAmOwner=Some(m##iAmOwner),
    ~previousAggregate,
    ~permissions=Some(permissions),
    ~recentMeasurement,
    (),
  );
};

let component = (~id, fn) => {
  let query = Query.make(~id, ());
  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.bind(_, e =>
           e##measurable
           |> filterOptionalResult("Measurable not found" |> ste)
         )
      |> E.R.fmap(toMeasurable)
      |> E.R.fmap(fn)
      |> E.R.id
    }
  </QueryComponent>;
};
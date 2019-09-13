open Utils;

let toMeasurable = (m): Types.measurable =>
  Primary.Measurable.make(
    ~id=m##id,
    ~name=m##name,
    ~valueType=m##valueType,
    ~channel=None,
    ~labelCustom=m##labelCustom,
    ~resolutionEndpoint=m##resolutionEndpoint,
    ~measurementCount=m##measurementCount,
    ~measurerCount=m##measurerCount,
    ~createdAt=Some(m##createdAt),
    ~updatedAt=Some(m##updatedAt),
    ~expectedResolutionDate=m##expectedResolutionDate,
    ~state=Some(m##state),
    ~stateUpdatedAt=m##stateUpdatedAt,
    ~labelSubject=m##labelSubject,
    ~labelOnDate=m##labelOnDate,
    ~labelProperty=m##labelProperty,
    ~channelId=m##channelId,
    ~min=m##min,
    ~max=m##max,
    (),
  );

module Query = [%graphql
  {|
      query getMeasurable ($id: String!) {
        measurable(id: $id) {
           id
           name
           labelCustom
           resolutionEndpoint
           valueType
           measurementCount
           measurerCount
           labelSubject
           labelProperty
           labelOnDate @bsDecoder(fn: "E.J.O.toMoment")
           state
           stateUpdatedAt @bsDecoder(fn: "E.J.O.toMoment")
           expectedResolutionDate @bsDecoder(fn: "E.J.O.toMoment")
           createdAt @bsDecoder(fn: "E.J.toMoment")
           updatedAt @bsDecoder(fn: "E.J.toMoment")
           channelId
           creator {
             id
             name
           }
           min
           max
        }
      }
    |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

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
      |> E.R.fmap(fn)
      |> E.R.id
    }
  </QueryComponent>;
};
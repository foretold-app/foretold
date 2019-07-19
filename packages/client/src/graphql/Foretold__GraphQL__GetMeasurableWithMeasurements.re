open Utils;

module Query = [%graphql
  {|
      query getMeasurable ($id: String!) {
          measurable:
            measurable(id: $id){
              id
              name
              valueType
              channelId
              labelCustom
              resolutionEndpoint
              resolutionEndpointResponse
              labelSubject
              labelProperty
              state
              labelOnDate @bsDecoder(fn: "E.J.O.toMoment")
              stateUpdatedAt @bsDecoder(fn: "E.J.O.toMoment")
              expectedResolutionDate @bsDecoder(fn: "E.J.O.toMoment")
              createdAt @bsDecoder(fn: "E.J.toMoment")
              updatedAt @bsDecoder(fn: "E.J.toMoment")
              creator {
                id
                name
              }
              series {
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

type measurableQuery = {
  .
  "createdAt": MomentRe.Moment.t,
  "creator":
    option({
      .
      "id": string,
      "name": option(string),
    }),
  "expectedResolutionDate": option(MomentRe.Moment.t),
  "id": string,
  "labelCustom": option(string),
  "labelOnDate": option(MomentRe.Moment.t),
  "labelProperty": option(string),
  "labelSubject": option(string),
  "name": string,
  "channelId": string,
  "resolutionEndpoint": option(string),
  "resolutionEndpointResponse": option(float),
  "series":
    option({
      .
      "id": string,
      "name": option(string),
    }),
  "state": Primary.MeasurableState.t,
  "valueType": Types.valueType,
  "stateUpdatedAt": option(MomentRe.Moment.t),
  "updatedAt": MomentRe.Moment.t,
  "min": option(float),
  "max": option(float),
};

let queryMeasurable = (m: measurableQuery): Primary.Measurable.t => {
  let agent: option(Primary.Agent.t) =
    m##creator
    |> E.O.fmap(r => Primary.Agent.make(~id=r##id, ~name=r##name, ()));

  let series: option(Primary.Series.t) =
    m##series
    |> E.O.fmap(r => Primary.Series.make(~id=r##id, ~name=r##name, ()));

  Primary.Measurable.make(
    ~id=m##id,
    ~name=m##name,
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
    (),
  );
};

let component = (~id, fn) => {
  let query = Query.make(~id, ());
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    |> E.R.bind(_, e =>
         e##measurable |> filterOptionalResult("Measurable not found" |> ste)
       )
    |> E.R.fmap(queryMeasurable)
    |> E.R.fmap(fn)
    |> E.R.id
  )
  |> E.React.el;
};
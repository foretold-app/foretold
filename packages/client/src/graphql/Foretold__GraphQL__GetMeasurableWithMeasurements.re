open Utils;

module Query = [%graphql
  {|
      query getMeasurable ($id: String!) {
          measurable:
            measurable(id: $id){
              id
              name
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
  "resolutionEndpoint": option(string),
  "resolutionEndpointResponse": option(float),
  "series":
    option({
      .
      "id": string,
      "name": option(string),
    }),
  "state": Context.Primary.MeasurableState.t,
  "stateUpdatedAt": option(MomentRe.Moment.t),
  "updatedAt": MomentRe.Moment.t,
};

let queryMeasurable = (m: measurableQuery): Context.Primary.Measurable.t => {
  let agent: option(Context.Primary.Agent.t) =
    m##creator
    |> E.O.fmap(r => Context.Primary.Agent.make(~id=r##id, ~name=r##name, ()));

  let series: option(Context.Primary.Series.t) =
    m##series
    |> E.O.fmap(r =>
         Context.Primary.Series.make(~id=r##id, ~name=r##name, ())
       );

  Context.Primary.Measurable.make(
    ~id=m##id,
    ~name=m##name,
    ~labelCustom=m##labelCustom,
    ~resolutionEndpoint=m##resolutionEndpoint,
    ~resolutionEndpointResponse=m##resolutionEndpointResponse,
    ~createdAt=Some(m##createdAt),
    ~updatedAt=Some(m##updatedAt),
    ~expectedResolutionDate=m##expectedResolutionDate,
    ~state=Some(m##state |> Context.Primary.MeasurableState.fromEnum),
    ~stateUpdatedAt=m##stateUpdatedAt,
    ~labelSubject=m##labelSubject,
    ~labelOnDate=m##labelOnDate,
    ~labelProperty=m##labelProperty,
    ~measurements=None,
    ~creator=agent,
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
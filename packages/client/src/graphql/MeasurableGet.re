open Utils;

type creator = {
  id: string,
  name: option(string),
};

type measurable = {
  id: string,
  name: string,
  valueType: Types.valueType,
  labelCustom: option(string),
  channelId: string,
  resolutionEndpoint: option(string),
  measurementCount: option(int),
  measurerCount: option(int),
  createdAt: MomentRe.Moment.t,
  updatedAt: MomentRe.Moment.t,
  expectedResolutionDate: option(MomentRe.Moment.t),
  state: Types.measurableState,
  stateUpdatedAt: option(MomentRe.Moment.t),
  creator: option(creator),
  labelSubject: option(string),
  labelOnDate: option(MomentRe.Moment.t),
  labelProperty: option(string),
  min: option(float),
  max: option(float),
};

let toMeasurable = (m: measurable): Types.measurable =>
  Primary.Measurable.make(
    ~id=m.id,
    ~name=m.name,
    ~valueType=m.valueType,
    ~channel=None,
    ~labelCustom=m.labelCustom,
    ~resolutionEndpoint=m.resolutionEndpoint,
    ~measurementCount=m.measurementCount,
    ~measurerCount=m.measurerCount,
    ~createdAt=Some(m.createdAt),
    ~updatedAt=Some(m.updatedAt),
    ~expectedResolutionDate=m.expectedResolutionDate,
    ~state=Some(m.state),
    ~stateUpdatedAt=m.stateUpdatedAt,
    ~labelSubject=m.labelSubject,
    ~labelOnDate=m.labelOnDate,
    ~labelProperty=m.labelProperty,
    ~channelId=m.channelId,
    ~min=m.min,
    ~max=m.max,
    (),
  );

module Query = [%graphql
  {|
      query getMeasurable ($id: String!) {
          measurable:
            measurable(id: $id) @bsRecord{
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
           state @bsDecoder(fn: "Primary.MeasurableState.fromEnum")
           stateUpdatedAt @bsDecoder(fn: "E.J.O.toMoment")
           expectedResolutionDate @bsDecoder(fn: "E.J.O.toMoment")
           createdAt @bsDecoder(fn: "E.J.toMoment")
           updatedAt @bsDecoder(fn: "E.J.toMoment")
           channelId
           creator @bsRecord{
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
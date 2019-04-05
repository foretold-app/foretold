open Utils;

type creator = {
  id: string,
  name: option(string),
};
type measurable = {
  id: string,
  name: string,
  valueType: Context.Primary.valueType,
  description: option(string),
  resolutionEndpoint: option(string),
  measurementCount: option(int),
  measurerCount: option(int),
  createdAt: MomentRe.Moment.t,
  updatedAt: MomentRe.Moment.t,
  expectedResolutionDate: option(MomentRe.Moment.t),
  state: Context.Primary.MeasurableState.t,
  stateUpdatedAt: option(MomentRe.Moment.t),
  creator: option(creator),
  descriptionEntity: option(string),
  descriptionDate: option(MomentRe.Moment.t),
  descriptionProperty: option(string),
};

let toMeasurable = (m: measurable): Context.Primary.Measurable.t =>
  Context.Primary.Measurable.make(
    ~id=m.id,
    ~name=m.name,
    ~valueType=m.valueType,
    ~channel=None,
    ~description=m.description,
    ~resolutionEndpoint=m.resolutionEndpoint,
    ~measurementCount=m.measurementCount,
    ~measurerCount=m.measurerCount,
    ~createdAt=Some(m.createdAt),
    ~updatedAt=Some(m.updatedAt),
    ~expectedResolutionDate=m.expectedResolutionDate,
    ~state=Some(m.state),
    ~stateUpdatedAt=m.stateUpdatedAt,
    ~descriptionEntity=m.descriptionEntity,
    ~descriptionDate=m.descriptionDate,
    ~descriptionProperty=m.descriptionProperty,
    (),
  );

module Query = [%graphql
  {|
      query getMeasurable ($id: String!) {
          measurable:
            measurable(id: $id) @bsRecord{
           id
           name
           description
           resolutionEndpoint
           valueType
           measurementCount
           measurerCount
           descriptionEntity
           descriptionProperty
           descriptionDate @bsDecoder(fn: "E.S.O.toMoment")
           state @bsDecoder(fn: "Context.Primary.MeasurableState.fromEnum")
           stateUpdatedAt @bsDecoder(fn: "E.S.O.toMoment")
           expectedResolutionDate @bsDecoder(fn: "E.S.O.toMoment")
           createdAt @bsDecoder(fn: "E.S.toMoment")
           updatedAt @bsDecoder(fn: "E.S.toMoment")
           creator @bsRecord{
             id
             name
           }
          }
      }
    |}
];
module QueryComponent = ReasonApollo.CreateQuery(Query);
let component = (~id, fn) => {
  let query = Query.make(~id, ());
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    |> E.R.bind(_, e =>
         e##measurable |> filterOptionalResult("Measurable not found" |> ste)
       )
    |> E.R.fmap(fn)
    |> E.R.id
  )
  |> E.React.el;
};
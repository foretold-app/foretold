open Rationale;
open QueriesHelper;
open Utils;

type measurable = {
  id: string,
  name: string,
  valueType: DataModel.valueType,
  description: option(string),
  resolutionEndpoint: option(string),
  createdAt: MomentRe.Moment.t,
  updatedAt: MomentRe.Moment.t,
  state: DataModel.measurableState,
  stateUpdatedAt: option(MomentRe.Moment.t),
  expectedResolutionDate: option(MomentRe.Moment.t),
  descriptionEntity: option(string),
  descriptionDate: option(MomentRe.Moment.t),
  descriptionProperty: option(string),
};
let toMeasurable = (m: measurable): DataModel.measurable =>
  DataModel.toMeasurable(
    ~id=m.id,
    ~name=m.name,
    ~valueType=m.valueType,
    ~description=m.description,
    ~resolutionEndpoint=m.resolutionEndpoint,
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

type measurables = array(measurable);

module Query = [%graphql
  {|
      query getAgent ($id: String!) {
          agent:
          agent(id: $id){
              id
              measurables: Measurables @bsRecord{
            id
            name
            description
            resolutionEndpoint
            valueType
            descriptionEntity
            descriptionProperty
            descriptionDate @bsDecoder(fn: "optionalMoment")
            state @bsDecoder(fn: "string_to_measurableState")
            stateUpdatedAt @bsDecoder(fn: "optionalMoment")
            expectedResolutionDate @bsDecoder(fn: "optionalMoment")
            createdAt @bsDecoder(fn: "toMoment")
            updatedAt @bsDecoder(fn: "toMoment")
              }
          }
      }
    |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let notFound = <h3> {"Agent not found" |> ste} </h3>;

let component = (~id, innerFn) => {
  let query = Query.make(~id, ());
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    |> E.R.fmap(e => e##agent)
    |> E.R.bind(_, e =>
         switch (e) {
         | Some(a) => Ok(a)
         | None => Error(notFound)
         }
       )
    |> E.R.fmap(innerFn)
    |> E.R.id
  )
  |> ReasonReact.element;
};
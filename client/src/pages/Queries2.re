open Rationale;
open Rationale.Option;
open Rationale.Function.Infix;
open MomentRe;
let jsonToString = e => e |> Js.Json.decodeString |> Option.default("");
let optionalMoment = Option.Infix.(e => e <$> (jsonToString ||> moment));
let toMoment = jsonToString ||> moment;

module GetUserMeasurables = {
  type measurable = {
    id: string,
    name: string,
    valueType: Queries.valueType,
    isLocked: bool,
    description: option(string),
    resolutionEndpoint: option(string),
    createdAt: MomentRe.Moment.t,
    updatedAt: MomentRe.Moment.t,
    expectedResolutionDate: option(MomentRe.Moment.t),
    lockedAt: option(MomentRe.Moment.t),
  };

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
            isLocked
            expectedResolutionDate @bsDecoder(fn: "optionalMoment")
            createdAt @bsDecoder(fn: "toMoment")
            updatedAt @bsDecoder(fn: "toMoment")
            lockedAt @bsDecoder(fn: "optionalMoment")
              }
          }
      }
    |}
  ];

  module QueryComponent = ReasonApollo.CreateQuery(Query);

  let toMeasurable = (m: measurable): Queries.measurable => {
    id: m.id,
    name: m.name,
    isLocked: m.isLocked,
    valueType: m.valueType,
    description: m.description,
    resolutionEndpoint: m.resolutionEndpoint,
    measurementCount: None,
    measurerCount: None,
    createdAt: m.createdAt,
    updatedAt: m.updatedAt,
    expectedResolutionDate: m.expectedResolutionDate,
    lockedAt: m.lockedAt,
    creator: None,
  };
};
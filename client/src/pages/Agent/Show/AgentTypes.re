open MomentRe;
open QueriesHelper;

/* let toOptionalMoment: option(Js.Json.t) => MomentRe.Moment.t = x => x <$> jsonToString |> Option.default("") |> moment */
let toOptionalMoment: option(Js.Json.t) => MomentRe.Moment.t =
  e =>
    (
      switch (e) {
      | Some(f) => f |> QueriesHelper.jsonToString
      | None => ""
      }
    )
    |> moment;

type user = {
  id: string,
  name: string,
};

type competitorType = [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE];
type bot = {
  competitorType,
  description: option(string),
  id: string,
  name: option(string),
};

type measurable = {
  id: string,
  name: string,
  state: DataModel.measurableState,
  stateUpdatedAt: option(MomentRe.Moment.t),
  expectedResolutionDate: option(MomentRe.Moment.t),
};

type measurement = {
  id: string,
  relevantAt: option(MomentRe.Moment.t),
  competitorType,
  description: option(string),
  value: Belt.Result.t(Value.t, string),
  createdAt: MomentRe.Moment.t,
  taggedMeasurementId: option(string),
  measurable: option(measurable),
};

type agent = {
  user: option(user),
  bot: option(bot),
  measurements: array(option(measurement)),
};

module GetAgent = {
  module Query = [%graphql
    {|
    query getAgent ($id: String!) {
        agent:
        agent(id: $id) @bsRecord{
        user: User @bsRecord{
          id
          name
        }
        bot: Bot @bsRecord{
          id
          name
          description
          competitorType
        }
        measurements: Measurements @bsRecord{
           id
           createdAt @bsDecoder(fn: "toMoment")
           relevantAt @bsDecoder(fn: "optionalMoment")
           value @bsDecoder(fn: "Value.decode")
           description
           competitorType
           taggedMeasurementId
           measurable: Measurable @bsRecord{
             id
             name
             expectedResolutionDate @bsDecoder(fn: "optionalMoment")
             state @bsDecoder(fn: "string_to_measurableState")
             stateUpdatedAt @bsDecoder(fn: "optionalMoment")
          }
        }
        }
    }
  |}
  ];

  module QueryComponent = ReasonApollo.CreateQuery(Query);

  let component = (~id, innerFn) => {
    open Rationale.Result.Infix;
    open Utils;
    open Rationale;
    let notFound = <h3> {"Agent not found" |> ste} </h3>;
    let query = Query.make(~id, ());
    QueryComponent.make(~variables=query##variables, ({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      <$> (e => e##agent)
      >>= (
        e =>
          switch (e) {
          | Some(a) => Ok(a)
          | None => Error(notFound)
          }
      )
      <$> innerFn
      |> Result.result(idd, idd)
    )
    |> ReasonReact.element;
  };
};

let toMeasurables = (measurements: array(measurement)) => {
  let r = measurements;
  let standardMeasurements =
    r
    |> E.A.fmap(n =>
         DataModel.toMeasurement(
           ~id=n.id,
           ~value=n.value,
           ~description=n.description,
           ~createdAt=Some(n.createdAt),
           ~competitorType=n.competitorType,
           ~relevantAt=n.relevantAt,
           ~measurableId=
             switch (n.measurable) {
             | Some(n) => Some(n.id)
             | None => None
             },
           (),
         )
       )
    |> E.A.to_list;

  let measurables =
    r
    |> E.A.fmap((t: measurement) => (t.measurable: option(measurable)))
    |> E.A.to_list
    |> Rationale.RList.filter_opt
    |> Rationale.RList.uniqBy((t: measurable) => t.id)
    |> List.map((e: measurable) =>
         DataModel.toMeasurable(
           ~id=e.id,
           ~name=e.name,
           ~expectedResolutionDate=e.expectedResolutionDate,
           ~state=Some(e.state),
           ~stateUpdatedAt=e.stateUpdatedAt,
           ~measurements=
             Some(
               standardMeasurements
               |> List.filter((s: DataModel.measurement) =>
                    s.measurableId == Some(e.id)
                  ),
             ),
           (),
         )
       );
  measurables;
};
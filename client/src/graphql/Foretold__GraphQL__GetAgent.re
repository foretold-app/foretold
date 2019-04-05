open MomentRe;

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
  state: Context.Primary.MeasurableState.t,
  stateUpdatedAt: option(MomentRe.Moment.t),
  expectedResolutionDate: option(MomentRe.Moment.t),
};

type measurement = {
  id: string,
  relevantAt: option(MomentRe.Moment.t),
  competitorType,
  description: option(string),
  value: Belt.Result.t(MeasurementValue.t, string),
  createdAt: MomentRe.Moment.t,
  taggedMeasurementId: option(string),
  measurable: option(measurable),
};

type agent = {
  user: option(user),
  bot: option(bot),
  measurements: array(option(measurement)),
};

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
           createdAt @bsDecoder(fn: "E.J.toMoment")
           relevantAt @bsDecoder(fn: "E.J.O.toMoment")
           value @bsDecoder(fn: "MeasurementValue.decode")
           description
           competitorType
           taggedMeasurementId
           measurable: Measurable @bsRecord{
             id
             name
             expectedResolutionDate @bsDecoder(fn: "E.J.O.toMoment")
             state @bsDecoder(fn: "Context.Primary.MeasurableState.fromEnum")
             stateUpdatedAt @bsDecoder(fn: "E.J.O.toMoment")
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
  let notFound = "Agent not found" |> ste;
  let query = Query.make(~id, ());
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    |> E.R.fmap(e => e##agent)
    |> E.R.bind(_, e =>
         switch (e) {
         | Some(a) => Ok(a)
         | None => Error(notFound |> E.React.inH3)
         }
       )
    |> E.R.fmap(innerFn)
    |> E.R.id
  )
  |> E.React.el;
};

let toMeasurables = (measurements: array(measurement)) => {
  let r = measurements;
  let standardMeasurements =
    r
    |> E.A.fmap(n =>
         Context.Primary.Measurement.make(
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
    |> E.L.filter_opt
    |> E.L.uniqBy((t: measurable) => t.id)
    |> E.L.fmap((e: measurable) =>
         Context.Primary.Measurable.make(
           ~id=e.id,
           ~name=e.name,
           ~expectedResolutionDate=e.expectedResolutionDate,
           ~state=Some(e.state),
           ~stateUpdatedAt=e.stateUpdatedAt,
           ~measurements=
             Some(
               standardMeasurements
               |> E.L.filter((s: Context.Primary.Measurement.t) =>
                    s.measurableId == Some(e.id)
                  ),
             ),
           (),
         )
       );
  measurables;
};
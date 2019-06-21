type user = {
  id: string,
  name: string,
};

type competitorType = [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE];
type bot = {
  competitorType,
  description: option(string),
  id: string,
  name: string,
};

type measurable = {
  id: string,
  name: string,
  state: Context.Primary.MeasurableState.t,
  stateUpdatedAt: option(MomentRe.Moment.t),
  expectedResolutionDate: option(MomentRe.Moment.t),
};

type connection('a) =
  option({. "edges": option(Js.Array.t(option({. "node": option('a)})))});

type node = {
  id: string,
  relevantAt: option(MomentRe.Moment.t),
  competitorType,
  description: option(string),
  createdAt: MomentRe.Moment.t,
  taggedMeasurementId: option(string),
  measurable: option(measurable),
  value: MeasurementValue.graphQlResult,
};

type agent = {
  user: option(user),
  bot: option(bot),
  measurements: connection(node),
  isMe: bool,
};

let unpackEdges = (a: connection('a)): array('a) => {
  a
  |> E.O.fmap(b => b##edges |> E.A.O.defaultEmpty |> E.A.O.concatSome)
  |> E.A.O.defaultEmpty
  |> E.A.fmap(e => e##node)
  |> E.A.O.concatSome;
};
/* |> E.O.fmap(b => b##edges |> E.A.O.defaultEmpty)
   |> E.O.toExn("Expected items"); */

module Query = [%graphql
  {|
    query getAgent ($id: String!) {
        agent:
        agent(id: $id) @bsRecord{
        isMe
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
        measurements: Measurements {
           edges {
             node @bsRecord{
              id
              createdAt @bsDecoder(fn: "E.J.toMoment")
              relevantAt @bsDecoder(fn: "E.J.O.toMoment")
              description
              competitorType
              taggedMeasurementId
              value {
                floatCdf {
                  xs
                  ys
                }
                floatPoint
              }
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
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let toMeasurements = (measurements: array(node)) => {
  let r = measurements;
  let standardMeasurements =
    r
    |> E.A.fmap(n =>
         Context.Primary.Measurement.make(
           ~id=n.id,
           ~value=n.value |> MeasurementValue.decodeGraphql,
           ~description=n.description,
           ~createdAt=Some(n.createdAt),
           ~competitorType=n.competitorType,
           ~relevantAt=n.relevantAt,
           ~measurableId=
             switch (n.measurable) {
             | Some(n) => Some(n.id)
             | None => None
             },
           ~measurable=
             switch (n.measurable) {
             | Some(m) =>
               Some(
                 Context.Primary.Measurable.make(~id=m.id, ~name=m.name, ()),
               )
             | None => None
             },
           (),
         )
       )
    |> E.A.to_list;
  standardMeasurements;
};

type response = {
  agent,
  measurementsList: list(Context.Primary.Measurement.t),
};

let component = (~id, innerFn) => {
  open Utils;
  let notFound = "Agent not found" |> ste;
  let query = Query.make(~id, ());
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    |> E.R.fmap(e => {
         let agent = e##agent;
         let measurementsEdges: option(array(node)) =
           agent |> E.O.fmap(agent => agent.measurements |> unpackEdges);
         let measurements = measurementsEdges |> E.O.fmap(toMeasurements);

         switch (agent, measurements) {
         | (Some(a), Some(b)) => Some({agent: a, measurementsList: b})
         | _ => None
         };
       })
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
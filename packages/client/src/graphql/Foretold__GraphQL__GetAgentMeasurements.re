type measurable = {
  id: string,
  name: string,
  state: Context.Primary.MeasurableState.t,
  stateUpdatedAt: option(MomentRe.Moment.t),
  expectedResolutionDate: option(MomentRe.Moment.t),
};

// @todo:
type connection('a) =
  option({
    .
    "edges": option(Js.Array.t(option({. "node": option('a)}))),
    "pageInfo": {
      .
      "endCursor": option(string),
      "hasNextPage": bool,
      "hasPreviousPage": bool,
      "startCursor": option(string),
    },
    "total": option(int),
  });

type node = {
  id: string,
  relevantAt: option(MomentRe.Moment.t),
  competitorType: Context.Primary.CompetitorType.t,
  description: option(string),
  createdAt: MomentRe.Moment.t,
  taggedMeasurementId: option(string),
  measurable: option(measurable),
  value: MeasurementValue.graphQlResult,
};

type agent = {measurements: connection(node)};

let unpackEdges = (a: connection('a)): array('a) => {
  a
  |> Rationale.Option.fmap(b =>
       b##edges |> E.A.O.defaultEmpty |> E.A.O.concatSome
     )
  |> E.A.O.defaultEmpty
  |> Array.map(e => e##node)
  |> E.A.O.concatSome;
};

module Query = [%graphql
  {|
    query getAgentMeasurements ($id: String!, $first: Int, $last: Int, $after: String, $before: String) {
        agent:
        agent(id: $id) @bsRecord{
            measurements: Measurements (first: $first, last: $last, after: $after, before: $before) {
               total
               pageInfo{
                 hasPreviousPage
                 hasNextPage
                 startCursor
                 endCursor
               }
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

let toMesuarements = (measurements: array(node)) => {
  measurements
  |> Array.map(n =>
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
     );
};

let unpackConnection = responseResult => {
  let agent = responseResult##agent;

  let measurementsEdges: option(array(node)) =
    agent |> Rationale.Option.fmap(agent => agent.measurements |> unpackEdges);

  let measurements =
    measurementsEdges
    |> Rationale.Option.fmap(toMesuarements)
    |> E.A.O.defaultEmpty;

  // @todo:
  let pageInfo =
    Context.Primary.PageInfo.fromJson({
      "endCursor": Some("end"),
      "hasNextPage": false,
      "hasPreviousPage": false,
      "startCursor": Some("start"),
    });

  Context.Primary.Connection.make(
    ~pageInfo,
    ~total=Some(10),
    ~edges=measurements,
  );
};

let componentMaker = (query, innerComponentFn) =>
  QueryComponent.make(~variables=query##variables, o =>
    o.result
    |> E.HttpResponse.fromApollo
    |> E.HttpResponse.fmap(unpackConnection)
    |> innerComponentFn
  )
  |> E.React.el;

type direction = Context.Primary.Connection.direction;

type inputType('a) =
  (
    ~id: string,
    ~first: int=?,
    ~last: int=?,
    ~after: string=?,
    ~before: string=?,
    unit
  ) =>
  'a;

let queryDirection = (~id, ~pageLimit, ~direction, ~fn: inputType('a), ()) => {
  let fn = fn(~id);
  switch ((direction: direction)) {
  | None => fn(~first=pageLimit, ())
  | After(after) => fn(~first=pageLimit, ~after, ())
  | Before(before) => fn(~last=pageLimit, ~before, ())
  };
};

let componentWithMeasurementConnection =
    (~id, ~pageLimit, ~direction: direction, ~innerComponentFn) => {
  let query = queryDirection(~id, ~pageLimit, ~direction, ~fn=Query.make, ());
  componentMaker(query, innerComponentFn);
};
open Utils;

module Types = {
  type competitorType = [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE];
  type measurement = {
    .
    "node":
      option({
        .
        "id": string,
        "agent":
          option({
            .
            "bot":
              option({
                .
                "id": string,
                "name": option(string),
                "competitorType": competitorType,
              }),
            "id": string,
            "name": option(string),
            "user":
              option({
                .
                "id": string,
                "name": string,
              }),
          }),
        "description": option(string),
        "relevantAt": option(MomentRe.Moment.t),
        "competitorType": competitorType,
        "createdAt": MomentRe.Moment.t,
        "taggedMeasurementId": option(string),
        "value": MeasurementValue.graphQlResult,
      }),
  };

  type measurements = option({. "edges": option(Js.Array.t(measurement))});
};

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
              measurements: Measurements{
                edges {
                  node{
                    id
                    createdAt @bsDecoder(fn: "E.J.toMoment")
                    value {
                      floatCdf {
                        xs
                        ys
                      }
                      floatPoint
                    }
                    relevantAt @bsDecoder(fn: "E.J.O.toMoment")
                    competitorType
                    description
                    taggedMeasurementId
                    agent: Agent {
                      id
                      name
                      user: User {
                        id
                        name
                      }
                      bot: Bot {
                        id
                        name
                        competitorType
                      }
                    }
                  }
                }
              }
          }
      }
    |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let toMeasurement = (m: Types.measurement): Context.Primary.Measurement.t => {
  open Context.Primary.Agent;
  let measurement = m##node |> E.O.toExn("FD");
  let agent = m##node |> E.O.bind(_, r => r##agent);
  let agentType: option(Context.Primary.AgentType.t) =
    agent
    |> E.O.bind(_, k =>
         switch (k##bot, k##user) {
         | (Some(bot), None) =>
           Some(
             Context.Primary.Types.Bot(
               Context.Primary.Bot.make(
                 ~id=bot##id,
                 ~name=bot##name,
                 ~competitorType=bot##competitorType,
                 (),
               ),
             ),
           )
         | (None, Some(user)) =>
           Some(
             Context.Primary.Types.User(
               Context.Primary.User.make(~id=user##id, ~name=user##name, ()),
             ),
           )
         | (_, _) => None
         }
       );

  let agent: option(Context.Primary.Agent.t) =
    agent
    |> E.O.fmap(k => Context.Primary.Agent.make(~id=k##id, ~agentType, ()));

  Context.Primary.Measurement.make(
    ~id=measurement##id,
    ~description=measurement##description,
    ~value=measurement##value |> MeasurementValue.decodeGraphql,
    ~competitorType=measurement##competitorType,
    ~taggedMeasurementId=measurement##taggedMeasurementId,
    ~createdAt=Some(measurement##createdAt),
    ~relevantAt=measurement##relevantAt,
    ~agent,
    (),
  );
};

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
  "measurements":
    option({. "edges": option(Js.Array.t(option(Types.measurement)))}),
  "name": string,
  "resolutionEndpoint": option(string),
  "resolutionEndpointResponse": option(float),
  "series":
    option({
      .
      "id": string,
      "name": option(string),
    }),
  "state": [ | `JUDGED | `JUDGEMENT_PENDING | `OPEN],
  "stateUpdatedAt": option(MomentRe.Moment.t),
  "updatedAt": MomentRe.Moment.t,
};

let queryMeasurable = (m: measurableQuery) => {
  open Context.Primary;
  let agent: option(Agent.t) =
    m##creator |> E.O.fmap(r => Agent.make(~id=r##id, ~name=r##name, ()));

  let series: option(Series.t) =
    m##series |> E.O.fmap(r => Series.make(~id=r##id, ~name=r##name, ()));

  let unpackEdges =
      (a: option({. "edges": option(Js.Array.t('a))})): Js.Array.t('a) =>
    a
    |> E.O.fmap(b => b##edges |> E.A.O.defaultEmpty)
    |> E.O.toExn("Expected items");

  let measurements =
    m##measurements
    |> unpackEdges
    |> E.A.O.concatSome
    |> E.A.fmap(toMeasurement)
    |> Array.to_list;

  let measurable: Measurable.t =
    Measurable.make(
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
      ~measurements=Some(measurements),
      ~creator=agent,
      ~series,
      (),
    );
  measurable;
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
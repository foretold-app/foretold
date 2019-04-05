open Utils;

module Types = {
  type competitorType = [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE];
  type measurement = {
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
    "value": Belt.Result.t(MeasurementValue.t, string),
  };
  type measurements = Js.Array.t(option(measurement));
};

module Query = [%graphql
  {|
      query getMeasurable ($id: String!) {
          measurable:
            measurable(id: $id){
              id
              name
              labelCustom
              valueType
              creatorId
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
                description
              }
              measurements: Measurements{
                id
                createdAt @bsDecoder(fn: "E.J.toMoment")
                value @bsDecoder(fn: "MeasurementValue.decode")
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
    |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let toMeasurement = (m: Types.measurement): Context.Primary.Measurement.t => {
  open Context.Primary.Agent;
  let agentType: option(Context.Primary.AgentType.t) =
    m##agent
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
    m##agent
    |> E.O.fmap(k => Context.Primary.Agent.make(~id=k##id, ~agentType, ()));

  Context.Primary.Measurement.make(
    ~id=m##id,
    ~description=m##description,
    ~value=m##value,
    ~competitorType=m##competitorType,
    ~taggedMeasurementId=m##taggedMeasurementId,
    ~createdAt=Some(m##createdAt),
    ~relevantAt=m##relevantAt,
    ~agent,
    (),
  );
};

let queryMeasurable = m => {
  open Context.Primary;
  let agent: option(Agent.t) =
    m##creator |> E.O.fmap(r => Agent.make(~id=r##id, ~name=r##name, ()));

  let series: option(Series.t) =
    m##series |> E.O.fmap(r => Series.make(~id=r##id, ~name=r##name, ()));

  let measurements: list(Measurement.t) =
    m##measurements
    |> E.A.O.concatSomes
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
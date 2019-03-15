open QueriesHelper;
open Utils;

module Query = [%graphql
  {|
      query getMeasurable ($id: String!) {
          measurable:
            measurable(id: $id){
              id
              name
              description
              valueType
              creatorId
              channel
              resolutionEndpoint
              resolutionEndpointResponse
              descriptionEntity
              descriptionProperty
              descriptionDate @bsDecoder(fn: "optionalMoment")
              state @bsDecoder(fn: "string_to_measurableState")
              stateUpdatedAt @bsDecoder(fn: "optionalMoment")
              expectedResolutionDate @bsDecoder(fn: "optionalMoment")
              createdAt @bsDecoder(fn: "toMoment")
              updatedAt @bsDecoder(fn: "toMoment")
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
                createdAt @bsDecoder(fn: "toMoment")
                value @bsDecoder(fn: "Value.decode")
                relevantAt @bsDecoder(fn: "optionalMoment")
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

let queryMeasurable = m => {
  open DataModel;
  let agent: option(DataModel.agent) =
    m##creator
    |> E.O.fmap(r => DataModel.toAgent(~id=r##id, ~name=r##name, ()));

  let series: option(DataModel.series) =
    m##series
    |> E.O.fmap(r => DataModel.toSeries(~id=r##id, ~name=r##name, ()));

  let measurable: DataModel.measurable =
    DataModel.toMeasurable(
      ~id=m##id,
      ~name=m##name,
      ~channel=m##channel,
      ~description=m##description,
      ~resolutionEndpoint=m##resolutionEndpoint,
      ~resolutionEndpointResponse=m##resolutionEndpointResponse,
      ~createdAt=Some(m##createdAt),
      ~updatedAt=Some(m##updatedAt),
      ~expectedResolutionDate=m##expectedResolutionDate,
      ~state=Some(m##state),
      ~stateUpdatedAt=m##stateUpdatedAt,
      ~descriptionEntity=m##descriptionEntity,
      ~descriptionDate=m##descriptionDate,
      ~descriptionProperty=m##descriptionProperty,
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
    |> E.R.fmap(fn)
    |> E.R.id
  )
  |> ReasonReact.element;
};

let toMeasurement = (m: MeasurableTypes.measurement): DataModel.measurement => {
  open DataModel;
  let agentType: option(DataModel.agentType) =
    m##agent
    |> E.O.bind(_, k =>
         switch (k##bot, k##user) {
         | (Some(bot), None) =>
           Some(
             Bot({
               id: bot##id,
               name: bot##name,
               competitorType: bot##competitorType,
               description: None,
             }),
           )
         | (None, Some(user)) =>
           Some(User({id: user##id, name: user##name}))
         | (_, _) => None
         }
       );

  let agent: option(DataModel.agent) =
    m##agent |> E.O.fmap(k => DataModel.toAgent(~id=k##id, ~agentType, ()));

  DataModel.toMeasurement(
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
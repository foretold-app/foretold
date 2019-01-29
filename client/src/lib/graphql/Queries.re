/* I'm really not sure why I needed to do the converters, and couldn't just import the files from DataModel.re */
open Rationale;
open QueriesHelper;
open Utils;

module GetAgents = {
  type user = {
    id: string,
    name: string,
  };

  let toUser = (u: user): DataModel.user => {id: u.id, name: u.name};

  type bot = {
    competitorType: [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE],
    description: option(string),
    id: string,
    name: option(string),
  };

  let toBot = (a: bot): DataModel.bot => {
    competitorType: a.competitorType,
    description: a.description,
    id: a.id,
    name: a.name,
  };

  type agent = {
    id: string,
    measurementCount: option(int),
    bot: option(bot),
    user: option(user),
  };

  let toAgent = (a: agent): DataModel.agent => {
    let agentType: option(DataModel.agentType) =
      switch (a.bot, a.user) {
      | (Some(bot), None) => Some(Bot(toBot(bot)))
      | (None, Some(user)) => Some(User(toUser(user)))
      | _ => None
      };
    {id: a.id, measurementCount: a.measurementCount, agentType};
  };

  type agents = array(agent);

  module Query = [%graphql
    {|
    query getAgents {
      agents @bsRecord{
        id
        measurementCount
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
      }
    }
  |}
  ];

  module QueryComponent = ReasonApollo.CreateQuery(Query);
};

module GetUserMeasurables = {
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
};

module GetMeasurable = {
  type creator = {
    id: string,
    name: option(string),
  };
  type measurable = {
    id: string,
    name: string,
    valueType: DataModel.valueType,
    description: option(string),
    resolutionEndpoint: option(string),
    measurementCount: option(int),
    measurerCount: option(int),
    createdAt: MomentRe.Moment.t,
    updatedAt: MomentRe.Moment.t,
    expectedResolutionDate: option(MomentRe.Moment.t),
    state: DataModel.measurableState,
    stateUpdatedAt: option(MomentRe.Moment.t),
    creator: option(creator),
  };

  let toMeasurable = (m: measurable): DataModel.measurable =>
    DataModel.toMeasurable(
      ~id=m.id,
      ~name=m.name,
      ~valueType=m.valueType,
      ~description=m.description,
      ~resolutionEndpoint=m.resolutionEndpoint,
      ~measurementCount=m.measurementCount,
      ~measurerCount=m.measurerCount,
      ~createdAt=Some(m.createdAt),
      ~updatedAt=Some(m.updatedAt),
      ~expectedResolutionDate=m.expectedResolutionDate,
      ~state=Some(m.state),
      ~stateUpdatedAt=m.stateUpdatedAt,
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
           state @bsDecoder(fn: "string_to_measurableState")
           stateUpdatedAt @bsDecoder(fn: "optionalMoment")
           expectedResolutionDate @bsDecoder(fn: "optionalMoment")
           createdAt @bsDecoder(fn: "toMoment")
           updatedAt @bsDecoder(fn: "toMoment")
           creator @bsRecord{
             id
             name
           }
          }
      }
    |}
  ];
  module QueryComponent = ReasonApollo.CreateQuery(Query);
};

module GetMeasurables = {
  type creator = {
    id: string,
    name: option(string),
  };

  let toCreator = (c: creator): DataModel.creator => {
    id: c.id,
    name: c.name,
  };

  type measurable = {
    id: string,
    name: string,
    valueType: DataModel.valueType,
    description: option(string),
    resolutionEndpoint: option(string),
    measurementCount: option(int),
    measurerCount: option(int),
    createdAt: MomentRe.Moment.t,
    updatedAt: MomentRe.Moment.t,
    expectedResolutionDate: option(MomentRe.Moment.t),
    state: DataModel.measurableState,
    stateUpdatedAt: option(MomentRe.Moment.t),
    creator: option(creator),
  };

  let toMeasurable = (m: measurable): DataModel.measurable =>
    DataModel.toMeasurable(
      ~id=m.id,
      ~name=m.name,
      ~valueType=m.valueType,
      ~description=m.description,
      ~resolutionEndpoint=m.resolutionEndpoint,
      ~measurementCount=m.measurementCount,
      ~measurerCount=m.measurerCount,
      ~createdAt=Some(m.createdAt),
      ~updatedAt=Some(m.updatedAt),
      ~expectedResolutionDate=m.expectedResolutionDate,
      ~state=Some(m.state),
      ~stateUpdatedAt=m.stateUpdatedAt,
      ~creator=Rationale.Option.fmap(toCreator, m.creator),
      (),
    );

  module Query = [%graphql
    {|
    query getMeasurables {
        measurables @bsRecord {
           id
           name
           description
           resolutionEndpoint
           valueType
           measurementCount
           measurerCount
           state @bsDecoder(fn: "string_to_measurableState")
           stateUpdatedAt @bsDecoder(fn: "optionalMoment")
           expectedResolutionDate @bsDecoder(fn: "optionalMoment")
           createdAt @bsDecoder(fn: "toMoment")
           updatedAt @bsDecoder(fn: "toMoment")
           creator @bsRecord{
             id
             name
           }
        }
    }
  |}
  ];

  module QueryComponent = ReasonApollo.CreateQuery(Query);
};
module GetUser = {
  module Query = [%graphql
    {|
    query user ($auth0Id: String) {
        user:
          user(auth0Id: $auth0Id)  {
            id
            auth0Id
            name
            agentId
            agent: Agent  {
              id
            }
        }
    }
  |}
  ];

  module QueryComponent = ReasonApollo.CreateQuery(Query);
};

module GetMeasurableWithMeasurements = {
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
              resolutionEndpoint
              resolutionEndpointResponse
              state @bsDecoder(fn: "string_to_measurableState")
              stateUpdatedAt @bsDecoder(fn: "optionalMoment")
              expectedResolutionDate @bsDecoder(fn: "optionalMoment")
              createdAt @bsDecoder(fn: "toMoment")
              updatedAt @bsDecoder(fn: "toMoment")
              creator {
                id
                name
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

  let withQuery = (~id, fn) => {
    let query = Query.make(~id, ());
    Result.Infix.(
      QueryComponent.make(~variables=query##variables, ({result}) =>
        result
        |> ApolloUtils.apolloResponseToResult
        >>= (
          e =>
            e##measurable
            |> filterOptionalResult("Measurable not found" |> ste)
        )
        |> Rationale.Result.fmap(fn)
        |> Result.result(idd, idd)
      )
      |> ReasonReact.element
    );
  };

  let queryMeasurable = m => {
    open DataModel;
    let creator: option(DataModel.creator) =
      m##creator |> Option.fmap(r => {id: r##id, name: r##name});

    let measurable: DataModel.measurable =
      DataModel.toMeasurable(
        ~id=m##id,
        ~name=m##name,
        ~description=m##description,
        ~resolutionEndpoint=m##resolutionEndpoint,
        ~resolutionEndpointResponse=m##resolutionEndpointResponse,
        ~createdAt=Some(m##createdAt),
        ~updatedAt=Some(m##updatedAt),
        ~expectedResolutionDate=m##expectedResolutionDate,
        ~state=Some(m##state),
        ~stateUpdatedAt=m##stateUpdatedAt,
        ~creator,
        (),
      );
    measurable;
  };

  let toMeasurement = (m: MeasurableTypes.measurement): DataModel.measurement => {
    open DataModel;
    let agentType: option(DataModel.agentType) =
      m##agent
      |> Rationale.Option.bind(_, k =>
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
      m##agent
      |> Rationale.Option.fmap(k =>
           {id: k##id, measurementCount: None, agentType}
         );

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
};
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
    open Result.Infix;
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
      |> E.R.id
    )
    |> ReasonReact.element;
  };
};

module GetSeries = {
  type series = {
    id: string,
    name: option(string),
    description: option(string),
  };

  let toSeries = (m: series): DataModel.series =>
    DataModel.toSeries(~id=m.id, ~name=m.name, ());

  module Query = [%graphql
    {|
      query getSeries ($id: String!) {
          series: series(id: $id) @bsRecord{
           id
           name
           description
          }
      }
    |}
  ];
  module QueryComponent = ReasonApollo.CreateQuery(Query);

  let component = (~id, innerFn) => {
    open Result.Infix;
    let query = Query.make(~id, ());
    QueryComponent.make(~variables=query##variables, ({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      <$> (e => e##series)
      <$> innerFn
      |> E.R.id
    )
    |> ReasonReact.element;
  };
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
    channel: option(string),
    resolutionEndpoint: option(string),
    measurementCount: option(int),
    measurerCount: option(int),
    createdAt: MomentRe.Moment.t,
    updatedAt: MomentRe.Moment.t,
    expectedResolutionDate: option(MomentRe.Moment.t),
    state: DataModel.measurableState,
    stateUpdatedAt: option(MomentRe.Moment.t),
    creator: option(creator),
    descriptionEntity: option(string),
    descriptionDate: option(MomentRe.Moment.t),
    descriptionProperty: option(string),
  };

  let toMeasurable = (m: measurable): DataModel.measurable =>
    DataModel.toMeasurable(
      ~id=m.id,
      ~name=m.name,
      ~valueType=m.valueType,
      ~channel=m.channel,
      ~description=m.description,
      ~resolutionEndpoint=m.resolutionEndpoint,
      ~measurementCount=m.measurementCount,
      ~measurerCount=m.measurerCount,
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
           descriptionEntity
           channel
           descriptionProperty
           descriptionDate @bsDecoder(fn: "optionalMoment")
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
  let component = (~id, fn) => {
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
        <$> fn
        |> Result.result(idd, idd)
      )
      |> ReasonReact.element
    );
  };
};

module GetMeasurables = {
  type series = {
    id: string,
    description: option(string),
    name: option(string),
  };

  let toSeries = (c: series): DataModel.series => {
    id: c.id,
    description: c.description,
    name: c.name,
  };

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
    channel: option(string),
    valueType: DataModel.valueType,
    description: option(string),
    resolutionEndpoint: option(string),
    measurementCount: option(int),
    measurerCount: option(int),
    descriptionEntity: option(string),
    createdAt: MomentRe.Moment.t,
    updatedAt: MomentRe.Moment.t,
    expectedResolutionDate: option(MomentRe.Moment.t),
    state: DataModel.measurableState,
    stateUpdatedAt: option(MomentRe.Moment.t),
    creator: option(creator),
    series: option(series),
    descriptionDate: option(MomentRe.Moment.t),
    descriptionProperty: option(string),
  };

  let toMeasurable = (m: measurable): DataModel.measurable =>
    DataModel.toMeasurable(
      ~id=m.id,
      ~name=m.name,
      ~channel=m.channel,
      ~valueType=m.valueType,
      ~description=m.description,
      ~resolutionEndpoint=m.resolutionEndpoint,
      ~measurementCount=m.measurementCount,
      ~measurerCount=m.measurerCount,
      ~descriptionEntity=m.descriptionEntity,
      ~descriptionDate=m.descriptionDate,
      ~descriptionProperty=m.descriptionProperty,
      ~createdAt=Some(m.createdAt),
      ~updatedAt=Some(m.updatedAt),
      ~expectedResolutionDate=m.expectedResolutionDate,
      ~state=Some(m.state),
      ~stateUpdatedAt=m.stateUpdatedAt,
      ~creator=E.O.fmap(toCreator, m.creator),
      ~series=E.O.fmap(toSeries, m.series),
      (),
    );

  module Query = [%graphql
    {|
    query getMeasurables ($offset: Int, $limit: Int, $channel: String, $seriesId: String) {
        measurables(offset: $offset, limit: $limit, channel: $channel, seriesId: $seriesId) @bsRecord {
           id
           name
           channel
           description
           resolutionEndpoint
           valueType
           measurementCount
           measurerCount
           descriptionEntity
           descriptionProperty
           descriptionDate @bsDecoder(fn: "optionalMoment")
           state @bsDecoder(fn: "string_to_measurableState")
           stateUpdatedAt @bsDecoder(fn: "optionalMoment")
           expectedResolutionDate @bsDecoder(fn: "optionalMoment")
           createdAt @bsDecoder(fn: "toMoment")
           updatedAt @bsDecoder(fn: "toMoment")
           creator @bsRecord{
             id
             name
           }
           series @bsRecord{
             id
             name
             description
           }
        }
    }
  |}
  ];

  module QueryComponent = ReasonApollo.CreateQuery(Query);

  let component = (channel, innerComponentFn: 'a => ReasonReact.reactElement) => {
    open Result.Infix;
    let query = Query.make(~offset=0, ~limit=200, ~channel, ());
    QueryComponent.make(~variables=query##variables, o =>
      o.result
      |> ApolloUtils.apolloResponseToResult
      <$> (d => d##measurables)
      <$> E.A.Optional.concatSomes
      <$> (d => d |> E.A.fmap(toMeasurable))
      <$> (e => innerComponentFn(e))
      |> Result.result(idd, idd)
    )
    |> ReasonReact.element;
  };

  let componentWithSeries =
      (channel, seriesId, innerComponentFn: 'a => ReasonReact.reactElement) => {
    open Result.Infix;
    let query = Query.make(~offset=0, ~limit=200, ~channel, ~seriesId, ());
    QueryComponent.make(~variables=query##variables, o =>
      o.result
      |> ApolloUtils.apolloResponseToResult
      <$> (d => d##measurables)
      <$> E.A.Optional.concatSomes
      <$> (d => d |> E.A.fmap(toMeasurable))
      <$> (e => innerComponentFn(e))
      |> Result.result(idd, idd)
    )
    |> ReasonReact.element;
  };
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

  let component =
      (
        auth0Id: option(string),
        innerComponentFn: 'a => ReasonReact.reactElement,
      ) =>
    switch (auth0Id) {
    | Some(auth) =>
      let query = Query.make(~auth0Id=auth, ());
      Rationale.Result.Infix.(
        QueryComponent.make(
          ~variables=query##variables, ~pollInterval=5000, ({result}) =>
          result
          |> ApolloUtils.apolloResponseToResult
          <$> (e => innerComponentFn(Some(e)))
          |> E.R.id
        )
        |> ReasonReact.element
      );
    | None => innerComponentFn(None)
    };
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
      m##creator |> E.O.fmap(r => {id: r##id, name: r##name});

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
        ~creator,
        (),
      );
    measurable;
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
      m##agent
      |> E.O.fmap(k => {id: k##id, measurementCount: None, agentType});

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

module CreateMeasurementMutation = {
  module Query = [%graphql
    {|
            mutation createMeasurement($value: SequelizeJSON!, $description: String!, $competitorType:competitorType!, $measurableId:String!) {
                createMeasurement(value: $value, description: $description, competitorType: $competitorType, measurableId:$measurableId) {
                  createdAt
                }
            }
    |}
  ];

  module Mutation = ReasonApollo.CreateMutation(Query);

  type competitorType = [ | `COMPETITIVE | `OBJECTIVE];

  let mutate =
      (
        mutation: Mutation.apolloMutation,
        measurableId: string,
        value: Value.t,
        competitorType: competitorType,
        description: string,
      ) => {
    let m =
      Query.make(
        ~measurableId,
        ~value=value |> Value.encode,
        ~description,
        ~competitorType,
        (),
      );
    mutation(
      ~variables=m##variables,
      ~refetchQueries=[|"getMeasurable"|],
      (),
    )
    |> ignore;
  };
};
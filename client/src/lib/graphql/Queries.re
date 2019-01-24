/* I'm really not sure why I needed to do the converters, and couldn't just import the files from DataModel.re */
open Rationale;
open Rationale.Option;
open Rationale.Function.Infix;
open MomentRe;
open QueriesHelper;

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
    id: a.id,
    measurementCount: a.measurementCount,
    bot: a.bot |> Rationale.Option.fmap(toBot),
    user: a.user |> Rationale.Option.fmap(toUser),
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
    isLocked: bool,
    description: option(string),
    resolutionEndpoint: option(string),
    createdAt: MomentRe.Moment.t,
    updatedAt: MomentRe.Moment.t,
    expectedResolutionDate: option(MomentRe.Moment.t),
    lockedAt: option(MomentRe.Moment.t),
  };
  let toMeasurable = (m: measurable): DataModel.measurable => {
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
    isLocked: bool,
    description: option(string),
    resolutionEndpoint: option(string),
    measurementCount: option(int),
    measurerCount: option(int),
    createdAt: MomentRe.Moment.t,
    updatedAt: MomentRe.Moment.t,
    expectedResolutionDate: option(MomentRe.Moment.t),
    lockedAt: option(MomentRe.Moment.t),
    creator: option(creator),
  };
  let toMeasurable = (m: measurable): DataModel.measurable => {
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
           isLocked
           measurementCount
           measurerCount
           expectedResolutionDate @bsDecoder(fn: "optionalMoment")
           createdAt @bsDecoder(fn: "toMoment")
           updatedAt @bsDecoder(fn: "toMoment")
           lockedAt @bsDecoder(fn: "optionalMoment")
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
  type measurable = {
    id: string,
    name: string,
    valueType: DataModel.valueType,
    isLocked: bool,
    description: option(string),
    resolutionEndpoint: option(string),
    measurementCount: option(int),
    measurerCount: option(int),
    createdAt: MomentRe.Moment.t,
    updatedAt: MomentRe.Moment.t,
    expectedResolutionDate: option(MomentRe.Moment.t),
    lockedAt: option(MomentRe.Moment.t),
    creator: option(creator),
  };

  let toMeasurable = (m: measurable): DataModel.measurable => {
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

  module Query = [%graphql
    {|
    query getMeasurables {
        measurables @bsRecord {
           id
           name
           description
           resolutionEndpoint
           valueType
           isLocked
           measurementCount
           measurerCount
           expectedResolutionDate @bsDecoder(fn: "optionalMoment")
           createdAt @bsDecoder(fn: "toMoment")
           updatedAt @bsDecoder(fn: "toMoment")
           lockedAt @bsDecoder(fn: "optionalMoment")
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
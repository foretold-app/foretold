open Rationale;
open Rationale.Option;
open Rationale.Function.Infix;
open MomentRe;

let stringOfcompetitorType = e =>
  switch (e) {
  | `AGGREGATION => "Aggregation"
  | `COMPETITIVE => "Competitive"
  | `OBJECTIVE => "Objective"
  };

type user = {
  id: string,
  name: string,
};

type bot = {
  competitorType: [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE],
  description: option(string),
  id: string,
  name: option(string),
};

type agent = {
  id: string,
  measurementCount: option(int),
  bot: option(bot),
  user: option(user),
};

type agents = array(agent);

module GetAgents = [%graphql
  {|
    query getAgents {
      agents @bsRecord {
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

module GetAgentsQuery = ReasonApollo.CreateQuery(GetAgents);

type valueType = [ | `DATE | `FLOAT | `PERCENTAGE];

type creator = {
  id: string,
  name: option(string),
};

type measurable = {
  id: string,
  name: string,
  valueType,
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

type measurables = array(measurable);

let jsonToString = e => e |> Js.Json.decodeString |> Option.default("");

let toMoment = jsonToString ||> moment;

let optionalMoment = Option.Infix.(e => e <$> (jsonToString ||> moment));

module GetMeasurables = [%graphql
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
module GetMeasurablesQuery = ReasonApollo.CreateQuery(GetMeasurables);

module GetMeasurable = {
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

module GetUser = [%graphql
  {|
    query user ($auth0Id: String) {
        user:
          user(auth0Id: $auth0Id) {
            id
            auth0Id
            name
            agentId
            agent: Agent{
              id
            }
        }
    }
  |}
];

module GetUserQuery = ReasonApollo.CreateQuery(GetUser);
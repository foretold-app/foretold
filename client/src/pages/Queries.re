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
  bot: option(bot),
  user: option(user),
};

type agents = array(agent);

module GetAgents = [%graphql
  {|
    query getAgents {
      agents @bsRecord {
        id
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

type measurable = {
  id: string,
  name: string,
  valueType,
  isLocked: bool,
  measurementCount: option(int),
  createdAt: MomentRe.Moment.t,
  updatedAt: MomentRe.Moment.t,
};

type measurables = array(measurable);

let jsonToString = e => e |> Js.Json.decodeString |> Option.default("");

let toMoment = jsonToString ||> moment;

module GetMeasurables = [%graphql
  {|
    query getMeasurables {
        measurables @bsRecord {
           id
           name
           valueType
           isLocked
           measurementCount
           createdAt @bsDecoder(fn: "toMoment")
           updatedAt @bsDecoder(fn: "toMoment")
        }
    }
  |}
];
module GetMeasurablesQuery = ReasonApollo.CreateQuery(GetMeasurables);
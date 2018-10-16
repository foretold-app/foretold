open Rationale;
open Rationale.Option;
open MomentRe;
open Utils;

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
  measurementCount: option(int),
  measurerCount: option(int),
  measurableTableId: option(string),
  measurableTableAttributes: option(Js.Dict.t(string)),
  createdAt: MomentRe.Moment.t,
  updatedAt: MomentRe.Moment.t,
  expectedResolutionDate: option(MomentRe.Moment.t),
  creator: option(creator),
};

let getFn = (e: 'a, fn) => {
  let item =
    MeasurableTables.Queries.find(e.measurableTableId |> Option.default(""));

  e.measurableTableAttributes
  <$> (
    x =>
      x
      |> MeasurableTableBase.toMap
      |> (
        item
        |> Option.toExn("Item not found. Perhaps record doesn't have Id")
        |> fn
      )
  )
  |> Option.default("");
};

/* let foo = MeasurableTable.Base.findType; */

type measurables = array(measurable);

let jsonToString = e => e |> Js.Json.decodeString |> Option.default("");

let toMoment = Rationale.Function.Infix.(jsonToString ||> moment);

let optionalMoment =
  Rationale.Function.Infix.(e => e <$> (jsonToString ||> moment));

let decodeDict = (ojson: option(Js.Json.t)) : option(Js.Dict.t(string)) =>
  switch (ojson) {
  | Some(e) => Some(Json.Decode.dict(Json.Decode.string, e))
  | None => None
  };

module GetMeasurables = [%graphql
  {|
    query getMeasurables {
        measurables @bsRecord {
           id
           name
           valueType
           isLocked
           measurementCount
           measurerCount
           measurableTableId
           measurableTableAttributes @bsDecoder(fn: "decodeDict")
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
module GetMeasurablesQuery = ReasonApollo.CreateQuery(GetMeasurables);

module GetMeasurables2 = [%graphql
  {|
    query getMeasurables($measurableTableId: String) {
        measurables(measurableTableId: $measurableTableId) @bsRecord {
           id
           name
           valueType
           isLocked
           measurementCount
           measurerCount
           measurableTableId
           measurableTableAttributes @bsDecoder(fn: "decodeDict")
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
module GetMeasurablesQuery2 = ReasonApollo.CreateQuery(GetMeasurables2);

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

module GetMeasurable = {
  module GraphQL = [%graphql
    {|
    query getMeasurable ($id: String!){
        measurable(id: $id) @bsRecord {
           id
           name
           valueType
           isLocked
           measurementCount
           measurerCount
           measurableTableId
           measurableTableAttributes @bsDecoder(fn: "decodeDict")
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
  module QueryComponent = ReasonApollo.CreateQuery(GraphQL);

  let withQuery = (~id, fn) => {
    let query = GraphQL.make(~id, ());
    Result.Infix.(
      QueryComponent.make(~variables=query##variables, ({result}) =>
        result
        |> apolloResponseToResult
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
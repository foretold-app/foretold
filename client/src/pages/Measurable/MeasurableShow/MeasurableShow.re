open Utils;
open Rationale;
open Rationale.Option.Infix;
open Rationale.Function.Infix;
open Result.Infix;
open Queries;
open MomentRe;
open MeasurableTypes;

let toMoment = jsonToString ||> moment;

/* let toOptionalMoment: option(Js.Json.t) => MomentRe.Moment.t = x => x <$> jsonToString |> Option.default("") |> moment */
let toOptionalMoment: option(Js.Json.t) => MomentRe.Moment.t =
  e =>
    (
      switch (e) {
      | Some(f) => f |> jsonToString
      | None => ""
      }
    )
    |> moment;

module GetMeasurable = [%graphql
  {|
    query getMeasurable ($id: String!) {
        measurable:
          measurable(id: $id){
            id
            name
            valueType
            isLocked
            createdAt @bsDecoder(fn: "toMoment")
            measurements: Measurements{
              createdAt @bsDecoder(fn: "toMoment")
              value @bsDecoder(fn: "Shared.Value.decode")
              competitorType
              taggedMeasurementId
              relevantAt @bsDecoder(fn: "toOptionalMoment")
              agent: Agent {
                id
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

module CreateMeasurement = [%graphql
  {|
            mutation createMeasurement($value: SequelizeJSON!, $competitorType:competitorType!, $measurableId:String!, $agentId:String!) {
                createMeasurement(value: $value, competitorType: $competitorType, measurableId:$measurableId, agentId:$agentId) {
                  createdAt
                }
            }
    |}
];

module GetMeasurableQuery = ReasonApollo.CreateQuery(GetMeasurable);
module CreateMeasurementMutation =
  ReasonApollo.CreateMutation(CreateMeasurement);
let component = ReasonReact.statelessComponent("Measurable");

let valueString = e =>
  switch (e) {
  | `FLOAT => "Float"
  | `DATE => "Date"
  | `PERCENTAGE => "Percentage"
  };

module Styles = {
  open Css;
  let sidebar =
    style([float(`left), left(px(0)), backgroundColor(hex("eee"))]);

  let body = style([marginLeft(px(200)), padding(px(30))]);
};

let vvvv =
  `FloatPercentiles(
    Shared.Value.FloatPercentiles.fromArray([|
      (25.0, 50.0),
      (50.0, 150.0),
      (75.0, 250.0),
    |]),
  )
  |> Shared.Value.encode;

let make = (~id: string, _children) => {
  ...component,
  render: _self => {
    let query = GetMeasurable.make(~id, ());
    <div>
      <div>
        (
          GetMeasurableQuery.make(
            ~variables=query##variables, ~pollInterval=50000, ({result}) =>
            result
            |> apolloResponseToResult
            >>= (
              e =>
                e##measurable
                |> filterOptionalResult("Measurable not found" |> ste)
            )
            <$> (
              e =>
                <div>
                  <MeasurableShowForm measurableId=id />
                  <h2> (e##name |> ReasonReact.string) </h2>
                  <h3>
                    (
                      (e##isLocked ? "Locked: True" : "Locked: False")
                      |> ReasonReact.string
                    )
                  </h3>
                  <CreateMeasurementMutation>
                    ...(
                         (mutation, _) => {
                           let mut =
                             CreateMeasurement.make(
                               ~measurableId=e##id,
                               ~agentId="c4aefed8-83c1-422d-9364-313071287758",
                               ~value=vvvv,
                               ~competitorType=`COMPETITIVE,
                               (),
                             );
                           <div>
                             <h3
                               onClick=(
                                 e =>
                                   mutation(
                                     ~variables=mut##variables,
                                     ~refetchQueries=[|"getMeasurable"|],
                                     (),
                                   )
                                   |> ignore
                               )>
                               (
                                 e##valueType
                                 |> valueString
                                 |> ReasonReact.string
                               )
                             </h3>
                             <MeasurableChart measurements=e##measurements />
                             <MeasurableTable measurements=e##measurements />
                           </div>;
                         }
                       )
                  </CreateMeasurementMutation>
                </div>
            )
            |> Result.result(idd, idd)
          )
          |> ReasonReact.element
        )
      </div>
    </div>;
  },
};
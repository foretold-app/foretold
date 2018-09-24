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
              id
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

module GetMeasurableQuery = ReasonApollo.CreateQuery(GetMeasurable);

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
                  <h2> (e##name |> ReasonReact.string) </h2>
                  <h3>
                    (
                      (e##isLocked ? "Locked: True" : "Locked: False")
                      |> ReasonReact.string
                    )
                  </h3>
                  <div>
                    <h3>
                      (e##valueType |> valueString |> ReasonReact.string)
                    </h3>
                    <MeasurableChart measurements=e##measurements />
                    <MeasurableShowForm measurableId=id />
                    <MeasurableTable measurements=e##measurements />
                  </div>
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
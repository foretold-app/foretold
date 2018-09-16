open Utils;
open Rationale;
open Rationale.Option.Infix;
open Rationale.Function.Infix;
open Result.Infix;
open Queries;
open HandsOnTable;
open MomentRe;

let toMoment = jsonToString ||> moment;

module GetMeasurable = [%graphql
  {|
    query getMeasurable ($id: String!) {
        measurable:
          measurable(id: $id){
            id
            name
            createdAt @bsDecoder(fn: "toMoment")
            measurements: Measurements{
              percentile25 @bsDecoder(fn: "Js.Float.toString")
              percentile50 @bsDecoder(fn: "Js.Float.toString")
              percentile75 @bsDecoder(fn: "Js.Float.toString")
              createdAt @bsDecoder(fn: "toMoment")
              agent: Agent {
                id
                type
                user: User {
                  id
                  name
                }
                bot: Bot {
                  id
                  name
                }
              }
            }
        }
    }
  |}
];

module GetMeasurableQuery = ReasonApollo.CreateQuery(GetMeasurable);

let component = ReasonReact.statelessComponent("Measurable");

let make = (~id: string, _children) => {
  ...component,
  render: _self => {
    let query = GetMeasurable.make(~id, ());
    <div>
      <Header />
      <h2> (ReasonReact.string("Measurable Page")) </h2>
      (
        GetMeasurableQuery.make(
          ~variables=query##variables, ~pollInterval=5000, ({result}) =>
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
                <MeasurableChart measurements=e##measurements />
                <MeasurableTable measurements=e##measurements />
              </div>
          )
          |> Result.result(idd, idd)
        )
        |> ReasonReact.element
      )
    </div>;
  },
};
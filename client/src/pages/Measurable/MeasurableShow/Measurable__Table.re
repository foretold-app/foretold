open Utils;
open Rationale;
open Rationale.Option.Infix;
open MomentRe;
open Table;

let component = ReasonReact.statelessComponent("Measurable__Table");

let make = (~measurements: MeasurableTypes.measurements, _children) => {
  ...component,
  render: _ => {
    let data =
      measurements
      |> ArrayOptional.concatSomes
      |> Array.map(Queries.GetMeasurableWithMeasurements.toMeasurement);

    <div className=MeasurementTableStyles.group>
      {data |> Array.to_list |> MeasurementsBlock.make}
    </div>;
  },
};
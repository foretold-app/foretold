open Utils;

let component = ReasonReact.statelessComponent("Measurable__Table");

let make = (~measurements: MeasurableTypes.measurements, _children) => {
  ...component,
  render: _ => {
    let data =
      measurements
      |> E.A.Optional.concatSomes
      |> E.A.fmap(Queries.GetMeasurableWithMeasurements.toMeasurement);

    <div className=MeasurementTableStyles.group>
      {data |> E.A.to_list |> MeasurementsBlock.make}
    </div>;
  },
};
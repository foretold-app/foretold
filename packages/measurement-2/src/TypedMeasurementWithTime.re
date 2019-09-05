type time = int;

let concatSome = (optionals: array(option('a))): array('a) =>
  optionals
  |> Js.Array.filter(Rationale.Option.isSome)
  |> Js.Array.map(
       Rationale.Option.toExn("Warning: This should not have happened"),
     );

let defaultEmpty = (o: option(array('a))): array('a) =>
  switch (o) {
  | Some(o) => o
  | None => [||]
  };

module TypedMeasurementWithTime = {
  type t('a) = {
    measurementValue: 'a,
    time,
  };
  type tss('a) = array(t('a));
  let make = (time, measurementValue) => {measurementValue, time};
  type ts =
    ValueType.T.t(
      tss(MeasurementValue.Cdf.t),
      tss(float),
      tss(bool),
      tss(MeasurementValue.Percentage.t),
      tss(MeasurementValue.UnresolvableResolution.t),
      tss(MeasurementValue.Comment.t),
    );
};

module MeasurementWithTime = {
  type t = {
    measurementValue: MeasurementValue.MeasurementValue.t,
    time,
  };
  type ts = array(t);

  let firstElementType = (ts: ts) =>
    ts
    |> Belt.Array.get(_, 0)
    |> Rationale.Option.map((r: t) =>
         r.measurementValue |> ValueType.TypeName.fromType
       );

  let toTypedMeasurementsWithTime =
      (intendedType: ValueType.TypeName.t, ts: ts)
      : TypedMeasurementWithTime.ts => {
    let transform = (t2, t3): TypedMeasurementWithTime.ts =>
      ts
      |> Belt.Array.map(_, (t: t) =>
           t.measurementValue
           |> t2
           |> Rationale.Option.map(measurementValue =>
                TypedMeasurementWithTime.{time: t.time, measurementValue}
              )
         )
      |> concatSome
      |> t3;
    ValueType.T.(
      switch (intendedType) {
      | `Cdf => transform(toCdf, r => `Cdf(r))
      | `Float => transform(toFloat, r => `Float(r))
      | `Binary => transform(toBinary, r => `Binary(r))
      | `Percentage => transform(toPercentage, r => `Percentage(r))
      | `UnresolvableResolution =>
        transform(toUnresolvableResolution, r => `UnresolvableResolution(r))
      | `Comment => transform(toComment, r => `Comment(r))
      }
    );
  };

  let toTypeOfFirstElement = (ts: ts) => {
    firstElementType(ts)
    |> Rationale.Option.map(toTypedMeasurementsWithTime(_, ts));
  };
};
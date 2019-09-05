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
    measurement: 'a,
    time,
  };
  type tss('a) = array(t('a));
  let make = (time, measurement) => {measurement, time};
  type ts = [
    | `Cdf(tss(MeasurementValue.Cdf.t))
    | `Float(tss(float))
    | `Binary(tss(bool))
    | `Percentage(tss(MeasurementValue.Percentage.t))
    | `UnresolvableResolution(tss(MeasurementValue.UnresolvableResolution.t))
    | `Comment(tss(MeasurementValue.Comment.t))
  ];
};

module MeasurementWithTime = {
  type t = {
    measurement: MeasurementValue.MeasurementValue.t,
    time,
  };
  type ts = array(t);

  let firstElementType = (ts: ts) =>
    ts
    |> Belt.Array.get(_, 0)
    |> Rationale.Option.map((r: t) =>
         r.measurement |> MeasurementValue.MeasurementValue.toTypeName
       );

  let toTypedMeasurementWithTime =
      (intendedType: MeasurementValue.MeasurementValue.typeNames, ts: ts)
      : TypedMeasurementWithTime.ts => {
    let transform = (t2, t3): TypedMeasurementWithTime.ts =>
      ts
      |> Belt.Array.map(_, (t: t) =>
           t.measurement
           |> t2
           |> Rationale.Option.map(measurement =>
                TypedMeasurementWithTime.{time: t.time, measurement}
              )
         )
      |> concatSome
      |> t3;
    MeasurementValue.MeasurementValue.(
      switch (intendedType) {
      | Cdf => transform(toCdf, r => `Cdf(r))
      | Float => transform(toFloat, r => `Float(r))
      | Binary => transform(toBinary, r => `Binary(r))
      | Percentage => transform(toPercentage, r => `Percentage(r))
      | UnresolvableResolution =>
        transform(toUnresolvableResolution, r => `UnresolvableResolution(r))
      | Comment => transform(toComment, r => `Comment(r))
      }
    );
  };
};
type time = float;

module T = {
  type t('a) = {
    measurementValue: 'a,
    time,
  };

  type ts('a) = array(t('a));

  type wrappedTs =
    MeasurementValueWrapper.T.t(
      ts(Cdf.t),
      ts(float),
      ts(bool),
      ts(Percentage.t),
      ts(MeasurementValue.UnresolvableResolution.t),
      ts(MeasurementValue.Comment.t),
    );

  let toStartAtDistribution = (finalTime, ts: ts('a)) =>
    StartAtDistribution.make(
      ~finalX=finalTime,
      ~pointXY=ts |> Array.map(r => (r.time, r.measurementValue)),
    );

  let make = (time, measurementValue) => {measurementValue, time};
};

module MeasurementWithTime = {
  type t = {
    measurementValue: MeasurementValue.t,
    time,
  };
  type ts = array(t);

  let firstElementType = (ts: ts) =>
    ts
    |> Belt.Array.get(_, 0)
    |> Rationale.Option.map((r: t) =>
         r.measurementValue |> MeasurementValueWrapper.T.toTypeName
       );

  let toTypedMeasurementsWithTime =
      (intendedType: MeasurementValueWrapper.Name.t, ts: ts): T.wrappedTs => {
    let transform = (t2, t3): T.wrappedTs =>
      ts
      |> Belt.Array.map(_, (t: t) =>
           t.measurementValue
           |> t2
           |> Rationale.Option.map(measurementValue =>
                T.{time: t.time, measurementValue}
              )
         )
      |> Utility.Array.concatSome
      |> t3;

    MeasurementValueWrapper.T.(
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
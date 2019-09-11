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
  type p = {
    measurementValue: MeasurementValue.t,
    time,
  };
  type t = p;
  type ts = array(t);

  module MeasurementWithTimeInput = {
    type t = p;
    let toMeasurement = t => t.measurementValue;
  };

  module Collection =
    MeasurementValue.MakeValueCollection(MeasurementWithTimeInput);
};
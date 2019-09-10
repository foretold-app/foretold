type ts('a) = array(MeasurementWithTime.t('a));

type t =
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
    ~pointXY=
      ts
      |> Array.map((r: MeasurementWithTime.t('a)) =>
           (r.time, r.measurementValue)
         ),
  );
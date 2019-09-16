module MeasurementWithTime: {
  type time = float;

  type _t('a) = {
    time,
    measurementValue: 'a,
  };

  module MeasurementWithTimeInput: {type t('a) = _t('a);};
  type t = MeasurementWithTimeInput.t(MeasurementValue.t);
  type ts = array(t);
  type l('a) = MeasurementWithTimeInput.t('a);
  type ls('a) = array(MeasurementWithTimeInput.t('a));
  let make: (~time: time, ~measurementValue: 'a) => _t('a);
};

type measurementWithTime('a) = MeasurementWithTime.l('a);
type measurementsWithTime('a) = MeasurementWithTime.ls('a);

type combination('a, 'b) = {
  agentPredictions: measurementsWithTime('a),
  marketPredictions: option(measurementsWithTime('a)),
  resolution: measurementWithTime('b),
};

type cdfCdf = combination(Cdf.t, Cdf.t);
type cdfFloat = combination(Cdf.t, float);
type percentagePercentage = combination(Percentage.t, Percentage.t);

type t = [
  | `CdfCdf(cdfCdf)
  | `CdfFloat(cdfFloat)
  | `PercentagePercentage(percentagePercentage)
];

let toStartAtDistribution:
  t =>
  StartAtDistribution.t(MeasurementWithTime.time, PredictionResolutionGroup.t);

let fromMeasurementCombination:
  (
    ~agentPredictions: MeasurementWithTime.ts,
    ~marketPredictions: option(MeasurementWithTime.ts)=?,
    ~resolution: MeasurementWithTime.t
  ) =>
  Belt.Result.t(t, string);
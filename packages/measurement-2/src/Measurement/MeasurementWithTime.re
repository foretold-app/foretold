type time = float;

type _t('a) = {
  time,
  measurementValue: 'a,
};

let make = (~time, ~measurementValue, ()) => {measurementValue, time};

module MeasurementWithTimeInput = {
  type t('a) = _t('a);
  let toMeasurement = (t: t('a)): 'a => t.measurementValue;
  let map = (t: t('a), fn: 'a => 'b) =>
    make(~time=t.time, ~measurementValue=fn(t.measurementValue), ());
};

module Ts = {
  include MeasurementValueCollectionFunctor.Make(MeasurementWithTimeInput);
  let toStartAtDistribution = (finalTime, ts: ls('a)) =>
    StartAtDistribution.make(
      ~finalX=finalTime,
      ~pointXY=
        ts
        |> Array.map(({time, measurementValue}) => (time, measurementValue)),
    );
};

include Ts;
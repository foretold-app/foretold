type time = float;

type abstract('a) = {
  time,
  measurementValue: 'a,
};

module Foo = {
  type t = abstract(MeasurementValue.t);
};

let make = (~time, ~measurementValue, ()) => {measurementValue, time};

module MeasurementWithTimeInput = {
  type t = Foo.t;
  let toMeasurement = t => t.measurementValue;
};

module Ts = {
  include MeasurementValueCollectionFunctor.Make(MeasurementWithTimeInput);

  let toStartAtDistribution = (finalTime, ts: Uniform.us) =>
    StartAtDistribution.make(
      ~finalX=finalTime,
      ~pointXY=ts |> Array.map(r => (r.time, r.measurementValue)),
    );
};
type t = Foo.t;
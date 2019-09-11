module TArray =
  MeasurementValueCollectionFunctor.Make({
    type t = MeasurementValue.t;
    let toMeasurement = t => t;
  });
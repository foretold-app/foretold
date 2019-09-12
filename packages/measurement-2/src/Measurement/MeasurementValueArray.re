module TArray =
  MeasurementValueCollectionFunctor.Make({
    type t('a) = 'a;
    let toMeasurement = t => t;
    let map = (t, fn) => fn(toMeasurement(t));
  });
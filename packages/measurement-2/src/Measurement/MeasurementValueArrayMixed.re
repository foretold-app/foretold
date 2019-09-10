type single = MeasurementValue.t;
type t = array(single);

let first = Belt.Array.get(_, 0);

let firstElementType = (t: t) =>
  t |> first |> Rationale.Option.map(MeasurementValueWrapper.T.toTypeName);

let toMeasurementValueArrayWrapped =
    (intendedType: MeasurementValueWrapper.Name.t, ts: t)
    : MeasurementValueArrayWrapped.t => {
  let transform = (t2, t3): MeasurementValueArrayWrapped.t =>
    ts
    |> Belt.Array.map(_, (t: single) => t |> t2)
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
} /* let allSameType = (t: t) => toMeasurementValueArrayWrapped |> Array.length*/;
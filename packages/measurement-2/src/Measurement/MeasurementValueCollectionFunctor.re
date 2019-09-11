module type T = {
  type t;
  let toMeasurement: t => MeasurementValue.t;
};

module Make = (C: T) => {
  type t = C.t;
  let hasType = (intendedType, t) =>
    t |> C.toMeasurement |> MeasurementValueWrapper.Name.toIsFn(intendedType);

  type ts = array(t);
  let first = (ts: ts) => Belt.Array.get(ts, 0);

  let firstElementType = (t: ts) =>
    t
    |> first
    |> Rationale.Option.map(C.toMeasurement)
    |> Rationale.Option.map(MeasurementValueWrapper.T.toTypeName);

  let filterOnIntendedType =
      (intendedType: MeasurementValueWrapper.Name.t, ts: ts) =>
    ts |> Belt.Array.keep(_, hasType(intendedType));

  let filterOnFirstElementType = (ts: ts) =>
    switch (ts |> firstElementType) {
    | Some(t) => filterOnIntendedType(t, ts)
    | None => [||]
    };

  let isAllSameType = (ts: ts) =>
    Belt.Array.length(ts) == Belt.Array.length(filterOnFirstElementType(ts));

  module CollectionWithSameMeasurementType = {
    type t = MeasurementValueWrapper.Uniform.u(ts);

    let wrapWithFirstType = (t: ts) =>
      t
      |> firstElementType
      |> Rationale.Option.map(MeasurementValueWrapper.Name.toWrapperFn)
      |> Rationale.Option.map(r => r(t));

    let make = (t: ts) => t |> filterOnFirstElementType |> wrapWithFirstType;
  };
};
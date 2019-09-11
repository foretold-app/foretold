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

  let length = Belt.Array.length;
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

  module Uniform = {
    type us = ts;
    type wrapped = MeasurementValueWrapper.Uniform.u(ts);

    let wrapWithType = (t: us) =>
      t
      |> firstElementType
      |> Rationale.Option.map(MeasurementValueWrapper.Name.toWrapperFn)
      |> Rationale.Option.map(r => r(t));

    let typeName = firstElementType;

    // TODO: Make should only validate they are all the same, and perhaps there is at least one element.
    let validateAllSame = (ts: ts) =>
      isAllSameType(ts)
        ? Belt.Result.Ok(ts) : Error("Must all be the same");

    let validateHasLength = (ts: ts) =>
      length(ts) > 0
        ? Belt.Result.Ok(ts) : Error("Must have at least one element");

    let make = (t: ts): Belt.Result.t(us, string) =>
      t |> validateAllSame |> Belt.Result.flatMap(_, validateHasLength);
  };
};
module type T = {
  type t('a);
  let map: (t('a), 'a => 'b) => t('b);
  let toMeasurement: t(MeasurementValue.t) => MeasurementValue.t;
};

module Make = (C: T) => {
  type l('a) = C.t('a);
  type ls('a) = array(C.t('a));

  type t = C.t(MeasurementValue.t);
  type ts = array(t);

  let hasType = (intendedType, t) =>
    t |> C.toMeasurement |> MeasurementValueWrapper.Name.toIsFn(intendedType);

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

  let lengthOfIntendedType =
      (intededType: MeasurementValueWrapper.Name.t, ts: ts) =>
    filterOnIntendedType(intededType, ts) |> Belt.Array.length;

  module Uniform = {
    type tu = ts;

    let typeName = (t: ts) =>
      t
      |> firstElementType
      |> Rationale.Option.toExn("This should be impossible.");

    // TODO: Make should only validate they are all the same, and perhaps there is at least one element.
    let validateAllSame = (ts: ts) =>
      isAllSameType(ts)
        ? Belt.Result.Ok(ts) : Error("Must all be the same");

    let validateHasLength = (ts: ts) =>
      length(ts) > 0
        ? Belt.Result.Ok(ts) : Error("Must have at least one element");

    let make = (t: ts): Belt.Result.t(tu, string) =>
      t |> validateAllSame |> Belt.Result.flatMap(_, validateHasLength);

    let fromT =
        (intendedType: MeasurementValueWrapper.Name.t, ts: ts)
        : Belt.Result.t(ts, string) =>
      ts |> filterOnIntendedType(intendedType) |> make;
  };

  module Unwrapped = {
    type t =
      MeasurementValueWrapper.withInsides(
        array(C.t(Cdf.t)),
        array(C.t(float)),
        array(C.t(bool)),
        array(C.t(Percentage.t)),
        array(C.t(MeasurementValue.UnresolvableResolution.t)),
        array(C.t(MeasurementValue.Comment.t)),
      );

    let fromUniform = (t: ts): t => {
      let fn = (fn1, fn2) =>
        t
        |> Array.map(r =>
             r
             |> C.toMeasurement
             |> fn1
             |> Rationale.Option.fmap(result => C.map(r, _ => result))
           )
        |> E.Array.concatSome
        |> fn2;

      MeasurementValueWrapper.T.(
        switch (Uniform.typeName(t)) {
        | `Cdf => fn(toCdf, r => `Cdf(r))
        | `Float => fn(toFloat, r => `Float(r))
        | `Binary => fn(toBinary, r => `Binary(r))
        | `Percentage => fn(toPercentage, r => `Percentage(r))
        | `UnresolvableResolution =>
          fn(toUnresolvable, r => `UnresolvableResolution(r))
        | `Comment => fn(toComment, r => `Comment(r))
        }
      );
    };

    let fromT = (intendedType: MeasurementValueWrapper.Name.t, ts: ts) =>
      Uniform.fromT(intendedType, ts) |> Belt.Result.map(_, fromUniform);
  };
};
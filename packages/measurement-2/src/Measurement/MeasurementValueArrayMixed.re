type single = MeasurementValue.t;
type t = array(single);

let first = Belt.Array.get(_, 0);

let firstElementType = (t: t) =>
  t |> first |> Rationale.Option.map(MeasurementValueWrapper.T.toTypeName);

let filterOnIntendedType =
    (intendedType: MeasurementValueWrapper.Name.t, ts: t): t => {
  ts
  |> Belt.Array.keep(_, (t: single) =>
       t |> MeasurementValueWrapper.Name.toIsFn(intendedType)
     );
};

let filterOnFirstElementType = (ts: t) =>
  switch (ts |> firstElementType) {
  | Some(t) => filterOnIntendedType(t, ts)
  | None => [||]
  };

let isAllSameType = (ts: t) =>
  Belt.Array.length(ts) == Belt.Array.length(filterOnFirstElementType(ts));
module Comment = {
  type t = [ | `Generic | `QuestionFeedback | `Update];

  let toString = (e: t): string =>
    switch (e) {
    | `Generic => "Generic"
    | `QuestionFeedback => "Question Feedback"
    | `Update => "Update"
    };

  let toMeasurement = (t: t) => `Comment(t);
};

module UnresolvableResolution = {
  type t = [ | `Ambiguous | `FalseConditional | `ResultNotAvailable | `Other];

  let toString = (e: t): string =>
    switch (e) {
    | `Ambiguous => "Ambiguous"
    | `FalseConditional => "FalseConditional"
    | `ResultNotAvailable => "ResultNotAvailable"
    | `Other => "Other"
    };
  let toMeasurement = (t: t) => `UnresolvableResolution(t);
};

module MeasurementValue = {
  type t =
    MeasurementValueWrapper.T.t(
      Cdf.t,
      float,
      bool,
      Percentage.t,
      UnresolvableResolution.t,
      Comment.t,
    );
};

module type ValueCollection = {
  type t;
  let toMeasurement: t => MeasurementValue.t;
};

module MakeValueCollection = (C: ValueCollection) => {
  type t = C.t;
  //todo
  let hasType = (intendedType, t) => true;

  type ts = array(t);
  let first = (ts: ts) => Belt.Array.get(ts, 0);

  let firstElementType = (t: ts) =>
    t
    |> first
    |> Rationale.Option.map(C.toMeasurement)
    |> Rationale.Option.map(MeasurementValueWrapper.T.toTypeName);

  let filterOnIntendedType =
      (intendedType: MeasurementValueWrapper.Name.t, ts: ts) =>
    ts
    |> Belt.Array.keep(_, (t: t) =>
         t
         |> C.toMeasurement
         |> MeasurementValueWrapper.Name.toIsFn(intendedType)
       );

  let filterOnFirstElementType = (ts: ts) =>
    switch (ts |> firstElementType) {
    | Some(t) => filterOnIntendedType(t, ts)
    | None => [||]
    };

  let isAllSameType = (ts: ts) =>
    Belt.Array.length(ts) == Belt.Array.length(filterOnFirstElementType(ts));
};

module TArrayInput = {
  type t = MeasurementValue.t;
  let toMeasurement = t => t;
};

module TArray = MakeValueCollection(TArrayInput);

type t = MeasurementValue.t;
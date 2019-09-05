open Belt.Result;

module UnresolvableResolution = {
  type t = [
    | `AMBIGUOUS
    | `FALSE_CONDITIONAL
    | `OTHER
    | `RESULT_NOT_AVAILABLE
  ];

  let toString = (e: t): string =>
    switch (e) {
    | `AMBIGUOUS => "AMBIGUOUS"
    | `FALSE_CONDITIONAL => "FALSE_CONDITIONAL"
    | `OTHER => "OTHER"
    | `RESULT_NOT_AVAILABLE => "RESULT_NOT_AVAILABLE"
    };
};

module Comment = {
  type t = [ | `GENERIC | `QUESTION_FEEDBACK | `UPDATE];

  let fromString = e =>
    switch (e) {
    | "GENERIC" => `GENERIC
    | "QUESTION_FEEDBACK" => `QUESTION_FEEDBACK
    | "UPDATE" => `UPDATE
    | _ => Js.Exn.raiseError("Invalid GraphQL Comment: " ++ e)
    };

  let toPublicString = (e: t): string =>
    switch (e) {
    | `GENERIC => "Generic"
    | `QUESTION_FEEDBACK => "Question Feedback"
    | `UPDATE => "Update"
    };
};

module Cdf = {
  type t = {
    xs: array(float),
    ys: array(float),
  };

  let verifyHasLength = ({xs, ys}) =>
    Array.length(xs) != Array.length(ys)
      ? Error("Arrays must have more than 0 elements.") : Ok({xs, ys});

  let verifySameLength = ({xs, ys}) =>
    Array.length(xs) == 0 || Array.length(ys) == 0
      ? Error("Array length must be the same.") : Ok({xs, ys});

  // todo: verify is increasing
  let make = ({xs, ys}) =>
    {xs, ys} |> verifyHasLength |> Rationale.Result.bind(_, verifySameLength);
};

module Percentage = {
  type t = float;

  let inverse = (t: t): t => 1. -. t;

  let verifyInRange = (f: float) =>
    if (f < 0. || f > 1.) {
      Error("Percentage must be between 0 and 1.");
    } else {
      Ok(f);
    };

  let make = (t: t) => verifyInRange(t);
};

module MeasurementValue = {
  type t = [
    | `Cdf(Cdf.t)
    | `Float(float)
    | `Binary(bool)
    | `Percentage(Percentage.t)
    | `UnresolvableResolution(UnresolvableResolution.t)
    | `Comment(Comment.t)
  ];

  let isX = (toXFn, t: t) => t |> toXFn |> Rationale.Option.isSome;

  let toCdf = (t: t) =>
    switch (t) {
    | `Cdf(a) => Some(a)
    | _ => None
    };

  let toFloat = (t: t) =>
    switch (t) {
    | `Float(a) => Some(a)
    | _ => None
    };

  let toBinary = (t: t) =>
    switch (t) {
    | `Binary(a) => Some(a)
    | _ => None
    };

  let toPercentage = (t: t) =>
    switch (t) {
    | `Percentage(a) => Some(a)
    | _ => None
    };

  let toUnresolvableResolution = (t: t) =>
    switch (t) {
    | `UnresolvableResolution(a) => Some(a)
    | _ => None
    };

  let toComment = (t: t) =>
    switch (t) {
    | `Comment(a) => Some(a)
    | _ => None
    };

  let isCdf = isX(toCdf);
  let isFloat = isX(toFloat);
  let isBinary = isX(toBinary);
  let isPercentage = isX(toPercentage);
  let isComment = isX(toComment);
};

module TypeName = {
  type t =
    | Cdf
    | Float
    | Binary
    | Percentage
    | UnresolvableResolution
    | Comment;

  let toWrapperFn = (t: t) =>
    switch (t) {
    | Cdf => (r => `Cdf(r))
    | Float => (r => `Float(r))
    | Binary => (r => `Binary(r))
    | Percentage => (r => `Percentage(r))
    | UnresolvableResolution => (r => `UnresolvableResolution(r))
    | Comment => (r => `Comment(r))
    };

  let fromMeasurementValue = (t: MeasurementValue.t): t =>
    switch (t) {
    | `Cdf(_) => Cdf
    | `Float(_) => Float
    | `Binary(_) => Binary
    | `Percentage(_) => Percentage
    | `UnresolvableResolution(_) => UnresolvableResolution
    | `Comment(_) => Comment
    };
};
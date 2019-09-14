module Comment = {
  [@genType]
  type t =
    | Generic
    | QuestionFeedback
    | Update;

  [@genType]
  let toString = (e: t): string =>
    switch (e) {
    | Generic => "Generic"
    | QuestionFeedback => "Question Feedback"
    | Update => "Update"
    };

  [@genType]
  let toMeasurement = (t: t) => `Comment(t);
};

module UnresolvableResolution = {
  [@genType]
  type t =
    | Ambiguous
    | FalseConditional
    | ResultNotAvailable
    | Other;

  [@genType]
  let toString = (e: t): string =>
    switch (e) {
    | Ambiguous => "Ambiguous"
    | FalseConditional => "False Conditional"
    | ResultNotAvailable => "Result NotAvailable"
    | Other => "Other"
    };

  [@genType]
  let toMeasurement = (t: t) => `UnresolvableResolution(t);
};

[@genType]
type t =
  MeasurementValueWrapper.T.t(
    Cdf.t,
    float,
    bool,
    Percentage.t,
    UnresolvableResolution.t,
    Comment.t,
  );
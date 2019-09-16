module Comment = {
  type t =
    | Generic
    | QuestionFeedback
    | Update;

  let toString = (e: t): string =>
    switch (e) {
    | Generic => "Generic"
    | QuestionFeedback => "Question Feedback"
    | Update => "Update"
    };

  let toMeasurement = (t: t) => `Comment(t);
};

module UnresolvableResolution = {
  [@genType]
  type t =
    | Ambiguous
    | FalseConditional
    | ResultNotAvailable
    | Other;

  let toString = (e: t): string =>
    switch (e) {
    | Ambiguous => "Ambiguous"
    | FalseConditional => "False Conditional"
    | ResultNotAvailable => "Result NotAvailable"
    | Other => "Other"
    };

  let toMeasurement = (t: t) => `UnresolvableResolution(t);
};

type t =
  MeasurementValueWrapper.T.t(
    Cdf.t,
    float,
    bool,
    Percentage.t,
    UnresolvableResolution.t,
    Comment.t,
  );
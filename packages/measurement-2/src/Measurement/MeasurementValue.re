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

type t = MeasurementValue.t;
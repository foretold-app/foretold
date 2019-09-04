module UnresolvableResolution = {
  type t = Types.unresolvableTypes;

  let fromString = e =>
    switch (e) {
    | "AMBIGUOUS" => `AMBIGUOUS
    | "FALSE_CONDITIONAL" => `FALSE_CONDITIONAL
    | "OTHER" => `OTHER
    | "RESULT_NOT_AVAILABLE" => `RESULT_NOT_AVAILABLE
    | _ => Js.Exn.raiseError("Invalid GraphQL Unresolvable Resolution: " ++ e)
    };

  let toString = (e: t): string =>
    switch (e) {
    | `AMBIGUOUS => "AMBIGUOUS"
    | `FALSE_CONDITIONAL => "FALSE_CONDITIONAL"
    | `OTHER => "OTHER"
    | `RESULT_NOT_AVAILABLE => "RESULT_NOT_AVAILABLE"
    };

  let toPublicString = (e: t): string =>
    switch (e) {
    | `AMBIGUOUS => "Ambiguous"
    | `FALSE_CONDITIONAL => "False Conditional"
    | `OTHER => "Other"
    | `RESULT_NOT_AVAILABLE => "Result Not Available"
    };
};
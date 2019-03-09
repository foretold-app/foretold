open MomentRe;
open Rationale.Function.Infix;
open Rationale;

let jsonToString = e => e |> Js.Json.decodeString |> E.O.default("");

let toMoment = jsonToString ||> moment;

let optionalMoment = Option.Infix.(e => e <$> (jsonToString ||> moment));

let string_to_measurableState = DataModel.string_to_measurableState;
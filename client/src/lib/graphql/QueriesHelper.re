open MomentRe;
open Rationale.Option;
open Rationale.Function.Infix;
open Rationale;

let jsonToString = e => e |> Js.Json.decodeString |> Option.default("");

let toMoment = jsonToString ||> moment;

let optionalMoment = Option.Infix.(e => e <$> (jsonToString ||> moment));
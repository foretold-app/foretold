open Utils;
open Rationale;
open Rationale.Function.Infix;
open Result.Infix;
open Queries;

let component = ReasonReact.statelessComponent("User");

let make = (~id: string, _children) => {
  ...component,
  render: _self => {
    let query = Queries.GetUser.make(~id, ());
    <div>
      <Header />
      <h2> (ReasonReact.string("User Page")) </h2>
      (
        Queries.GetUserQuery.make(~variables=query##variables, ({result}) =>
          result
          |> apolloResponseToResult
          >>= ((e => e##user) ||> filterOptionalResult("not found" |> ste))
          <$> (e => e##name |> ste)
          |> Result.result(idd, idd)
        )
        |> ReasonReact.element
      )
    </div>;
  },
};
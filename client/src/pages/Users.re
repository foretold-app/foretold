open Utils;
open Rationale;
open Result.Infix;
open Rationale.Function.Infix;
open Queries;

let showQueryList = (~data: Js.Array.t('a), ~urlFn, ~render) =>
  ReasonReact.array(
    Array.mapi(
      (index, element) =>
        <div
          key=(string_of_int(index))
          onClick=(_event => element |> urlFn |> ReasonReact.Router.push)>
          (render(element))
        </div>,
      data,
    ),
  );

let userList =
  showQueryList(
    ~urlFn=user => "/users/" ++ user##id,
    ~render=e => e##name |> ste,
  );

let component = ReasonReact.statelessComponent("Users");

let make = _children => {
  ...component,
  render: _ =>
    Queries.GetUsersQuery.make(o =>
      o.result
      |> apolloResponseToResult
      <$> (d => d##users)
      <$> catOptionals
      <$> userList(~data=_)
      |> Result.result(idd, idd)
    )
    |> ReasonReact.element
    |> NormalLayout.make(~name="Users Page")
    |> ReasonReact.element,
};
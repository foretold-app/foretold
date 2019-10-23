[@bs.config {jsx: 3}];

open Css;

let fnWithDefault = (fn, r) =>
  r |> FC__E.O.fmap(e => Css.style(fn(e))) |> FC__E.O.default("");

/* TODO: Instead of accepting styles, this should accept "classNames" and use Css.merge */

[@react.component]
let make =
    (
      ~styles=[],
      ~className="",
      ~flex=?,
      ~flexDirection=?,
      ~float=?,
      ~onClick=_ => (),
      ~children,
    ) => {
  let flexStyle = flex |> fnWithDefault(e => [Css.flex(e)]);
  let floatStyle = float |> fnWithDefault(e => [Css.float(e)]);
  let directionStyle =
    flexDirection
    |> fnWithDefault(e => [display(`flex), Css.flexDirection(e)]);
  let allStyles =
    Css.merge([flexStyle, directionStyle, floatStyle, className, ...styles]);

  <div className=allStyles onClick> children </div>;
};

module Jsx2 = {
  let make =
      (
        ~styles=?,
        ~className=?,
        ~flex=?,
        ~flexDirection=?,
        ~float=?,
        ~onClick=?,
        children,
      ) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(
        ~styles?,
        ~className?,
        ~flex?,
        ~flexDirection?,
        ~float?,
        ~onClick?,
        ~children,
        (),
      ),
      children,
    );
};

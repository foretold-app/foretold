open Css;

let fnWithDefault = (fn, r) =>
  r |> FC__E.O.fmap(e => Css.style(fn(e))) |> FC__E.O.default("");

let component = ReasonReact.statelessComponent(__MODULE__);
let make =
    (
      ~styles=[],
      ~flex=?,
      ~flexDirection=?,
      ~float=?,
      ~onClick=_ => (),
      children,
    ) => {
  ...component,
  render: _ => {
    let flexStyle = flex |> fnWithDefault(e => [Css.flex(e)]);
    let floatStyle = float |> fnWithDefault(e => [Css.float(e)]);
    let directionStyle =
      flexDirection
      |> fnWithDefault(e => [display(`flex), Css.flexDirection(e)]);
    let allStyles =
      Css.merge([flexStyle, directionStyle, floatStyle, ...styles]);
    <div className=allStyles onClick> ...children </div>;
  },
};
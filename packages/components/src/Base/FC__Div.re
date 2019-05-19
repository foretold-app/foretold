open Css;

let component = ReasonReact.statelessComponent("divWithStyles");
let make = (~styles=[], ~flex=?, ~flexDirection=?, ~float=?, children) => {
  ...component,
  render: _ => {
    let flexStyle =
      flex
      |> FC__E.O.fmap(e => Css.style([Css.flex(e)]))
      |> FC__E.O.default("");
    let floatStyle =
      float
      |> FC__E.O.fmap(e => Css.style([Css.float(e)]))
      |> FC__E.O.default("");
    let directionStyle =
      flexDirection
      |> FC__E.O.fmap(e =>
           Css.style([display(`flex), Css.flexDirection(e)])
         )
      |> FC__E.O.default("");
    let allStyles =
      Css.merge([flexStyle, directionStyle, floatStyle, ...styles]);
    <div className=allStyles> ...children </div>;
  },
};
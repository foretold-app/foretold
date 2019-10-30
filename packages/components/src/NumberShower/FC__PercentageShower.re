[@bs.config {jsx: 3}];

let percentageSign = Css.(style([opacity(0.5), marginLeft(`em(0.1))]));

[@react.component]
let make = (~percentage, ~precision) => {
  let numberWithPresentation =
    FC__NumberShower.JS.numberShow(percentage, precision);
  <span>
    {FC__NumberShower.JS.valueGet(numberWithPresentation)
     |> ReasonReact.string}
    <span className=percentageSign> {"%" |> ReasonReact.string} </span>
  </span>;
};

module Jsx2 = {
  let make = (~percentage, ~precision, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~percentage, ~precision, ()),
      children,
    );
};

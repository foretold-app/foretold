let component = ReasonReact.statelessComponent(__MODULE__);
let percentageSign = Css.(style([opacity(0.5), marginLeft(`em(0.1))]));
let make = (~percentage, ~precision, _children) => {
  ...component,
  render: _ => {
    let numberWithPresentation =
      FC__NumberShower.JS.numberShow(percentage, precision);
    <span>
      {FC__NumberShower.JS.valueGet(numberWithPresentation)
       |> ReasonReact.string}
      <span className=percentageSign> {"%" |> ReasonReact.string} </span>
    </span>;
  },
};
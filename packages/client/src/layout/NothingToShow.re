open Utils;

module Styles = {
  open Css;
  let centerBlock = style([textAlign(`center), margin(`em(2.))]);
  let clear = style([clear(`both)]);
};

let component = ReasonReact.statelessComponent("NothingToShow");

let make = _children => {
  ...component,
  render: _ =>
    <>
      <div className=Styles.clear />
      <div className=Styles.centerBlock>
        {"Nothing to show" |> Utils.ste}
      </div>
    </>,
};
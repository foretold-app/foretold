let component = ReasonReact.statelessComponent("AgentLink");

module Styles = {
  open Css;
  let mainText = style([color(FC__Colors.accentBlue)]);
};

let make =
  FC__Link.make(
    ~colors=(FC__Colors.accentBlue, FC__Colors.darkLink),
    ~isDisabled=false,
  );
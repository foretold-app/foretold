let component = ReasonReact.statelessComponent(__MODULE__);

let make =
  FC__Link.make(
    ~colors=(FC__Colors.accentBlue, FC__Colors.darkLink),
    ~isDisabled=false,
  );
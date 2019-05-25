let component = ReasonReact.statelessComponent(__MODULE__);

let make =
  FC__Link.make(
    ~className=
      Css.(
        style([
          color(FC__Colors.accentBlue),
          hover([color(FC__Colors.darkLink)]),
        ])
      ),
    ~isDisabled=false,
  );
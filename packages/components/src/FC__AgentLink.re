let component = ReasonReact.statelessComponent(__MODULE__);

let make =
  FC__Link.make(
    ~className=
      Css.(
        style([
          color(FC__Settings.accentBlue),
          hover([color(FC__Settings.darkLink)]),
        ])
      ),
    ~isDisabled=false,
  );
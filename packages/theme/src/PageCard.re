let component = ReasonReact.statelessComponent(__MODULE__);

let link =
  BaseLink.make(
    ~colors=(Colors.accentBlue, Colors.darkAccentBlue),
    ~styles=Css.[marginRight(`em(2.)), fontWeight(`num(400))],
  );

let make = children => {
  ...component,
  render: _self =>
    <Div
      styles=[
        Css.(
          style(
            [
              background(Colors.white),
              border(`px(1), `solid, `hex("D5D7DA")),
              borderRadius(`px(5)),
            ]
            @ Base.fullWidthFloatLeft,
          )
        ),
      ]>
      ...children
    </Div>,
};

let defaultPadding = Css.padding2(~v=`em(0.0), ~h=`em(1.5));

let header = children =>
  <Div
    styles=[
      Css.(
        style(
          [borderBottom(`px(1), `solid, Colors.accentBlueO8)]
          @ Base.fullWidthFloatLeft,
        )
      ),
    ]>
    children
  </Div>;

module H1 = {
  let component = ReasonReact.statelessComponent("H1");
  let make = children => {
    ...component,
    render: _self =>
      <h1
        className=Css.(
          style(
            [
              fontSize(`px(21)),
              fontWeight(`num(500)),
              marginTop(`em(0.0)),
            ]
            @ Base.fullWidthFloatLeft,
          )
        )>
        ...children
      </h1>,
  };
};

module P = {
  let component = ReasonReact.statelessComponent("P");
  let make = children => {
    ...component,
    render: _self =>
      <p className=Css.(style([color(Colors.Text.LightBackground.p)]))>
        ...children
      </p>,
  };
};
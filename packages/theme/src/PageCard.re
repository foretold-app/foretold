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
          [borderBottom(`px(1), `solid, `hex("D5D7DAAA")), defaultPadding]
          @ Base.fullWidthFloatLeft,
        )
      ),
    ]>
    children
  </Div>;

let subheader = children =>
  <Div
    styles=[
      Css.(
        style(
          [
            background(`hex("F9FAFB")),
            borderBottom(`px(1), `solid, `hex("D5D7DAAA")),
            defaultPadding,
          ]
          @ Base.fullWidthFloatLeft,
        )
      ),
    ]>
    children
  </Div>;
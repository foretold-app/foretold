open FC__Base;

module Styles = {
  open Css;
  let outer =
    style(
      [
        backgroundColor(Colors.white),
        borderBottom(`px(1), `solid, Colors.accentBlueO8),
      ]
      @ BaseStyles.fullWidthFloatLeft,
    );

  let inner =
    style(
      [boxSizing(`borderBox), padding2(~v=`em(1.), ~h=`em(2.0))]
      @ BaseStyles.fullWidthFloatLeft,
    );

  let actionButton =
    style([
      border(`px(1), `solid, `hex("c7cfdb")),
      background(Colors.white),
      Css.transition(~duration=100, "background"),
      selector(":hover", [background(Colors.lightGrayBackground)]),
    ]);

  let actionButtonPosition =
    style([
      BaseStyles.floatRight,
      marginLeft(`em(2.)),
      marginTop(`em(0.5)),
      borderRadius(`px(5)),
      padding2(~v=`em(0.5), ~h=`em(2.0)),
    ]);
};

let actionButton =
  Link.make(
    ~colors=(Colors.link, Colors.link),
    ~isDisabled=false,
    ~styles=Css.merge([Styles.actionButtonPosition, Styles.actionButton]),
  );

let component = ReasonReact.statelessComponent(__MODULE__);

let make = children => {
  ...component,
  render: _self =>
    <Div styles=[Styles.outer]>
      <Div styles=[Styles.inner]> ...children </Div>
    </Div>,
};

module SubHeader = {
  open Colors.Text;
  let component = ReasonReact.statelessComponent(__MODULE__ ++ " Subheader");

  let make = children => {
    ...component,
    render: _self =>
      <Div
        styles=[
          Css.(
            style(
              [backgroundColor(Colors.lighterGrayBackground)]
              @ BaseStyles.fullWidthFloatLeft,
            )
          ),
        ]>
        <Div
          styles=[
            Css.(
              style(
                [
                  padding2(~v=`em(0.0), ~h=`em(2.0)),
                  borderBottom(`px(1), `solid, FC__Colors.border),
                ]
                @ BaseStyles.fullWidthFloatLeft,
              )
            ),
          ]>
          ...children
        </Div>
      </Div>,
  };
};
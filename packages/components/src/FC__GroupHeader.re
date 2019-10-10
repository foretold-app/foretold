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
      [boxSizing(`borderBox), padding2(~v=`em(0.5), ~h=`em(2.0))]
      @ BaseStyles.fullWidthFloatLeft,
    );

  let actionButtonPosition =
    style([
      BaseStyles.floatRight,
      marginLeft(`em(2.)),
      marginTop(`em(0.2)),
    ]);
};

let actionButton = (~variant: FC__Button.variant=Primary) =>
  FC__Button.make(
    ~variant,
    ~isDisabled=false,
    ~size=Medium,
    ~className=Css.(merge([Styles.actionButtonPosition])),
  );

let component = ReasonReact.statelessComponent(__MODULE__);

let make = children => {
  ...component,
  render: _self =>
    <Div.Jsx2 styles=[Styles.outer]>
      <Div.Jsx2 styles=[Styles.inner]> ...children </Div.Jsx2>
    </Div.Jsx2>,
};

module SubHeader = {
  let component = ReasonReact.statelessComponent(__MODULE__ ++ " Subheader");

  let make = children => {
    ...component,
    render: _self =>
      <Div.Jsx2
        styles=[
          Css.(
            style(
              [backgroundColor(Colors.lighterGrayBackground)]
              @ BaseStyles.fullWidthFloatLeft,
            )
          ),
        ]>
        <Div.Jsx2
          styles=[
            Css.(
              style(
                [
                  padding2(~v=`em(0.0), ~h=`em(2.0)),
                  borderBottom(`px(1), `solid, FC__Settings.border),
                ]
                @ BaseStyles.fullWidthFloatLeft,
              )
            ),
          ]>
          ...children
        </Div.Jsx2>
      </Div.Jsx2>,
  };
};
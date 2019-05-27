open FC__Base;

let styles = (~isDisabled=false, ~heightPadding=2, ()) => {
  let main =
    Css.(
      style([
        padding2(~v=`px(heightPadding), ~h=`px(14)),
        BaseStyles.floatLeft,
        borderRadius(Colors.BorderRadius.medium),
        border(`px(1), `solid, Colors.accentBlueO8),
      ])
    );
  let disabledStyles = Css.(style([background(Colors.greydisabled)]));
  isDisabled ? Css.merge([disabledStyles, main]) : main;
};

let component = ReasonReact.statelessComponent("Tab2");
let make = (~isActive, ~onClick=?, ~number: option(int)=?, children) => {
  ...component,
  render: _self => {
    let textStyle =
      Css.(
        style([
          BaseStyles.floatLeft,
          marginRight(`em(0.5)),
          marginTop(`px(3)),
        ])
      );

    let colors =
      Colors.Text.(
        isActive ?
          Css.[
            color(LightBackground.active),
            hover([color(LightBackground.active)]),
          ] :
          Css.[
            color(LightBackground.main),
            hover([color(LightBackground.active)]),
          ]
      );

    <Link
      ?onClick
      className=Css.(
        style([BaseStyles.floatLeft, marginRight(`em(1.8))] @ colors)
      )
      isDisabled=false>
      <span className=textStyle> ...children </span>
      {
        number
        |> FC__E.O.React.fmapOrNull(number =>
             <span className={styles()}>
               {number |> string_of_int |> ReasonReact.string}
             </span>
           )
      }
    </Link>;
  },
};
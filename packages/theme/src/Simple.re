let tagStyles = (~isDisabled=false, ~heightPadding=2, ()) => {
  let main =
    Css.[
      padding2(~v=`px(heightPadding), ~h=`px(14)),
      Base.floatLeft,
      borderRadius(`px(7)),
      border(`px(1), `solid, Colors.accentBlueO8),
    ];
  let disabledStyles = Css.[background(Colors.greydisabled)];
  isDisabled ? disabledStyles @ main : main;
};

module Linktag = {
  let make = BaseLink.make(~styles=tagStyles(), ~isDisabled=false);
};

let activeItemWithNumber = (isActive, text, number: int) => {
  let textStyle =
    Css.[Base.floatLeft, marginRight(`em(1.0)), marginTop(`px(3))];

  let colors =
    isActive ?
      (
        Colors.Text.LightBackground.active,
        Colors.Text.LightBackground.active,
      ) :
      (Colors.Text.LightBackground.main, Colors.Text.LightBackground.active);

  <BaseLink
    colors
    styles=Css.[Base.floatLeft, padding2(~v=`em(1.), ~h=`em(1.5))]
    isDisabled=false>
    <span className={Css.style(textStyle)}> text </span>
    <span className={Css.style(tagStyles())}>
      {number |> string_of_int |> ReasonReact.string}
    </span>
  </BaseLink>;
};
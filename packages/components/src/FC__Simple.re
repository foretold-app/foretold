open FC__Base;

let tagStyles = (~isDisabled=false, ~heightPadding=2, ()) => {
  let main =
    Css.[
      padding2(~v=`px(heightPadding), ~h=`px(14)),
      BaseStyles.floatLeft,
      borderRadius(`px(5)),
      border(`px(1), `solid, Colors.accentBlueO8),
    ];
  let disabledStyles = Css.[background(Colors.greydisabled)];
  isDisabled ? disabledStyles @ main : main;
};

module Linktag = {
  let make =
    Link.make(~styles=tagStyles(~heightPadding=4, ()), ~isDisabled=false);
};

let activeItemWithNumber = (isActive, text, number: int) => {
  let textStyle =
    Css.[BaseStyles.floatLeft, marginRight(`em(0.7)), marginTop(`px(3))];

  let colors =
    isActive ?
      (
        Colors.Text.LightBackground.active,
        Colors.Text.LightBackground.active,
      ) :
      (Colors.Text.LightBackground.main, Colors.Text.LightBackground.active);

  <Link
    colors
    styles=Css.[BaseStyles.floatLeft, padding2(~v=`em(0.7), ~h=`em(1.2))]
    isDisabled=false>
    <span className={Css.style(textStyle)}> text </span>
    <span className={Css.style(tagStyles())}>
      {number |> string_of_int |> ReasonReact.string}
    </span>
  </Link>;
};
open FC__Base;

let tagStyles = (~isDisabled=false, ~heightPadding=2, ()) => {
  let main =
    Css.(
      style([
        padding2(~v=`px(heightPadding), ~h=`px(14)),
        BaseStyles.floatLeft,
        borderRadius(`px(5)),
        border(`px(1), `solid, Colors.accentBlueO8),
      ])
    );
  let disabledStyles = Css.(style([background(Colors.greydisabled)]));
  isDisabled ? Css.merge([disabledStyles, main]) : main;
};

module Linktag = {
  let make =
    Link.make(~styles=tagStyles(~heightPadding=4, ()), ~isDisabled=false);
};

let activeItemWithNumber = (isActive, text, number: int) => {
  let textStyle =
    Css.(
      style([
        BaseStyles.floatLeft,
        marginRight(`em(0.7)),
        marginTop(`px(3)),
      ])
    );

  let colors =
    isActive ?
      (
        Colors.Text.LightBackground.active,
        Colors.Text.LightBackground.active,
      ) :
      (Colors.Text.LightBackground.main, Colors.Text.LightBackground.active);

  <Link
    colors
    styles=Css.(
      style([BaseStyles.floatLeft, padding2(~v=`em(0.7), ~h=`em(1.2))])
    )
    isDisabled=false>
    <span className=textStyle> text </span>
    <span className={tagStyles()}>
      {number |> string_of_int |> ReasonReact.string}
    </span>
  </Link>;
};
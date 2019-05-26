let styles = (~isDisabled=false, ~heightPadding=4, ()) => {
  let main =
    Css.(
      style([
        padding2(~v=`px(heightPadding), ~h=`px(14)),
        FC__Base.BaseStyles.floatLeft,
        borderRadius(FC__Colors.BorderRadius.medium),
        border(`px(1), `solid, FC__Colors.accentBlueO8),
      ])
    );
  let disabledStyles = Css.(style([background(FC__Colors.greydisabled)]));
  isDisabled ? Css.merge([disabledStyles, main]) : main;
};

let component = ReasonReact.statelessComponent(__MODULE__);

let make = (~href=?, ~onClick=?, ~isDisabled=false, ~className="", children) => {
  ...component,
  render: _self =>
    <FC__Link
      ?href
      ?onClick
      isDisabled
      className={Css.merge([styles(~isDisabled, ()), className])}>
      ...children
    </FC__Link>,
};
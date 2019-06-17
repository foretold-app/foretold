let component = ReasonReact.statelessComponent(__MODULE__);

module Styles = {
  open Css;
  module Colors = FC__Colors;

  // Reverting to "rc-dropdown" brings back some styles from the original css file
  let prefixCls = "ft-dropdown";
  let itemVerticalPadding = 5;
  let itemHorizontalPadding = 12;
  let bgColor = Colors.white;
  let textColor = Colors.textDarker;
  let hoverColor = `hex("ebfaff");
  let textSize = `rem(0.8);
  let textLineHeight = `em(1.3);

  FC__Menu.Styles.createStyle(
    ~prefixCls,
    ~itemVerticalPadding,
    ~itemHorizontalPadding,
    ~textColor,
    ~bgColor,
    ~hoverColor,
    ~textSize,
    ~textLineHeight,
    (),
  );

  let dropdownTrigger =
    style([
      border(`px(1), `solid, FC__Colors.border),
      borderRadius(`px(3)),
      // Subtracting 2 from horizontal padding to account for border
      padding2(
        ~v=`px(itemVerticalPadding),
        ~h=`px(itemHorizontalPadding - 2),
      ),
      backgroundColor(bgColor),
      fontSize(textSize),
      lineHeight(textLineHeight),
      fontFamily(FC__Colors.Text.standardFont),
      color(textColor),
      // Selector for trigger element with overlay open
      selector(
        "&." ++ prefixCls ++ "-open",
        [borderColor(`hex("40a9ff")), color(`hex("40a9ff"))],
      ),
    ]);
};

let make = (~title, ~trigger=FC__Dropdown.Hover, children) => {
  ...component,
  render: _self => {
    let overlay =
      switch (Array.length(children)) {
      | 1 => children[0]
      | _ => <div> ...children </div>
      };
    <FC__Dropdown trigger overlay prefixCls=Styles.prefixCls>
      <button className=Styles.dropdownTrigger>
        <span> {title |> React.string} </span>
        <FC__Icon.DownArrow />
      </button>
    </FC__Dropdown>;
  },
};

let component = ReasonReact.statelessComponent(__MODULE__);

module Styles = {
  open Css;
  module Colors = FC__Colors;

  let itemVerticalPadding = 5;
  let itemHorizontalPadding = 12;
  // Reverting to "rc-dropdown" brings back some styles from the original css file
  let prefixCls = "rc-dropdown";
  let prefixPlus = ext => "." ++ prefixCls ++ "-" ++ ext;
  let textColor = Colors.textDarker;
  let bgColor = Colors.white;
  let textSize = `rem(0.8);
  let textLineHeight = `em(1.5);

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
      fontFamily("Lato"),
      color(textColor),
      // Selector for trigger element with overlay open
      selector("&.rc-dropdown-open", [
        borderColor(`hex("40a9ff")),
        color(`hex("40a9ff"))
      ])
    ]);

  // Root
  // Dropdown root element. This, like submenu popups, are placed
  // in generated divs right inside document body element. Ie only
  // influenced by top level styles.
  // Note that submenus get their own top level element, so this
  // does not apply to submenus
  // Note: There is a style in FC__Dropdown that takes care
  // of position absolute and zindex
  //global("." ++ prefixCls, []);

  // Menu ul
  // <ul> element that contain the menu items in it's <li>'s
  // Both root menu and submenus have this class.
  // This ul element will receive the -hidden class when hidden
  // Move from FC__Menu.Styles > "ft-menu-general" when styles
  // need variation
  global(
    "ul" ++ prefixPlus("menu"),
    [
      lineHeight(textLineHeight),
      backgroundColor(bgColor),
      // Menu-item
      selector("li" ++ prefixPlus("menu-item"), [
        fontSize(textSize),
        color(textColor),
        selector(":hover", [backgroundColor(`hex("ebfaff"))]),
        padding2(~v=`px(itemVerticalPadding), ~h=`px(itemHorizontalPadding)),
        // Reversing outside element padding and adding it on the <a>
        // Not sure how needed this will be (taken from ant)
        selector(
          ">a",
          [
            display(`block),
            margin2(
              ~v=`px(- itemVerticalPadding),
              ~h=`px(- itemHorizontalPadding),
            ),
            padding2(
              ~v=`px(itemVerticalPadding),
              ~h=`px(itemHorizontalPadding),
            ),
          ],
        ),
      ]),
      // Submenu <li> pointer
      selector(">li" ++ prefixPlus("menu-submenu"), [
        fontSize(textSize),
        color(textColor),
        selector(":hover", [backgroundColor(`hex("ebfaff"))]),
      ]),
      // Divider
      // Can't move to general because the selector
      // is prefix specific
      selector(">li" ++ prefixPlus("menu-item-divider"),
      [
        margin2(~v=`px(4), ~h=`zero),
        height(`px(1)),
        backgroundColor(`hex("e5e5e5")), // A little lighter than border I think
        lineHeight(`zero),
        overflow(`hidden),
        padding(`zero),
      ]
      )
    ],
  );

  // Hidden menu
  // Class applied to submenu <ul>'s when menu is hidden. Elements are not initially
  // hidden, but added to the dom on demand. Therefore it is quirky to rely
  // on this class with regards to animations/transitions.
  // For transitions, see api of rc-dropdown/rc-menu
  global(prefixPlus("menu-hidden"), [display(`none)]);
  // Hidden root menu
  global(prefixPlus("hidden"), [display(`none)]);

  // Submenu
  // Submenu has a title element inside a div, and
  // a right arrow added with unicode
  global(
    prefixPlus("menu-submenu-title"),
    [
      padding4(
        ~top=`px(itemVerticalPadding),
        ~right=`px(itemHorizontalPadding * 2 + 5),
        ~bottom=`px(itemVerticalPadding),
        ~left=`px(itemHorizontalPadding),
      ),
    ],
  );
  global(
    prefixPlus("menu-submenu-arrow"),
    [
      position(`absolute),
      right(`px(itemHorizontalPadding)),
      selector(
        "::before",
        [
          color(Colors.textMedium),
          fontStyle(`normal),
          fontSize(`em(0.9)),
          // One option here is to use font-awesome,
          // don't know how much use it would be elsewhere or how to do this,
          // rc-menu does this by default
          contentRule({js|\u25B6|js}),
        ],
      ),
    ],
  );
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

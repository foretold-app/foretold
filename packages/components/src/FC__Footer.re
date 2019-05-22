open FC__Base;

let str = ReasonReact.string;

module Styles = {
  open Css;
  /* PaddingTop is applied to copyright and link items to get
     space between lines when wrapped.
     Both also have horizontal margin to ensure minimum space between items,
     this is then subtracted from at the edges of the "sections"-class. */
  let itemsPaddingTop = `em(1.2);
  let betweenItemsMargin = 0.8;

  /* Layout box for spacing in the page */
  let layoutBox = style(
    [
      margin2(`em(2.), `zero),
      padding2(`zero, `em(2.))
    ]
    @ BaseStyles.fullWidthFloatLeft
  );
  
  /* footerBox inside layoutBox, here the horizontal border is in line with the text.
     PaddingTop is added here instead of "itemsPaddingTop" to decrease line distance
     when wrap and make items appear more as a group */
  let footerBox = style(
    [
      borderTop(`px(1), `solid, Colors.border),
      paddingTop(`em(0.3)),
      fontSize(`em(0.9)),
      fontWeight(`bold),
      color(Colors.textMedium),
    ]
  );

  /* Sections enables wrapping of copyright and links */
  let sections = style(
    [
      display(`flex),
      flexWrap(`wrapReverse),
      justifyContent(`spaceBetween),
      margin2(`zero, `em(-.betweenItemsMargin))
    ]
  );

  let copyright = style(
    [
      flexGrow(10.),
      paddingTop(itemsPaddingTop),
      paddingRight(`em(0.2)),
      margin2(`zero, `em(betweenItemsMargin))
    ]
  );

  /* Some extra marginBottom is added to reinforce the links as
     a group vs copyright.
     The element grows a little bit to add some spacing for larger screens */
  let links = style(
    [
      display(`flex),
      flexWrap(`wrap),
      flexGrow(1.),
      media("(min-width: 720px)", [
        justifyContent(`spaceBetween)
      ]),
      listStyleType(`none),
      padding(`zero),
      margin(`zero),
      marginBottom(`em(1.2)),
      selector("li", [
        paddingTop(itemsPaddingTop),
        margin2(`zero, `em(betweenItemsMargin))
      ])
    ]
  );
};

let component = ReasonReact.statelessComponent(__MODULE__);

let make = (_children) => {
  ...component,
  render: _self =>
    <div className=Styles.layoutBox>
      <div className=Styles.footerBox>
        <div className=Styles.sections>
          <div className=Styles.copyright>
            {str({js|2019 \u00a9 Foretold|js})}
          </div>
          <ul className=Styles.links>
            <li> {str("About")} </li>
            <li> {str("Help")} </li>
            <li> {str("Documentation")} </li>
            <li> {str("Privacy Policy")} </li>
            <li> {str("Terms of Service")} </li>
          </ul>
        </div>
      </div>
    </div>,
};
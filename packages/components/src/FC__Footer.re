open FC__Base;

let rString = ReasonReact.string;

module Styles = {
  open Css;
  let layoutBox = style(
    [
      marginTop(`em(2.)),
      marginBottom(`em(2.)),
      paddingLeft(`em(2.)),
      paddingRight(`em(2.)),
    ]
    @ BaseStyles.fullWidthFloatLeft
  );
  
  let footerBox = style(
    [
      borderTop(`px(1), `solid, Colors.border),
      paddingTop(`em(1.5)),
      fontSize(`em(0.9)),
      fontWeight(`bold),
      color(Colors.textMedium)
    ]
  );

  let sections = style(
    [
      display(`flex),
      flexWrap(`wrap)
    ]
  );

  let copyright = style(
    [
      flexGrow(4.),
      flexShrink(1),
      flexBasis(auto)
    ]
  );

  let links = style(
    [
      display(`flex),
      listStyleType(`none),
      margin(`zero),
      flexGrow(1.),
      flexShrink(1),
      flexBasis(auto),
      selector("li", [
        marginLeft(`em(0.6)),
        textAlign(`right),
        flexGrow(1.),
        flexShrink(1),
        flexBasis(auto),
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
            {rString({js|2019 \u00a9 Foretold|js})}
          </div>
          <ul className=Styles.links>
            <li> {rString("About")} </li>
            <li> {rString("Help")} </li>
            <li> {rString("Documentation")} </li>
            <li> {rString("Privacy Policy")} </li>
            <li> {rString("Terms of Service")} </li>
          </ul>
        </div>
      </div>
    </div>,
};
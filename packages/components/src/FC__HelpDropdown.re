[@bs.config {jsx: 3}];

type content = {
  headerContent: ReasonReact.reactElement,
  bodyContent: ReasonReact.reactElement,
};

module Overlay = {
  module Styles = {
    open Css;
    let className = style([maxWidth(`em(30.))]);
  };

  [@react.component]
  let make = (~content) =>
    <div className=Styles.className>
      <FC__PageCard>
        <FC__PageCard.HeaderRow>
          <FC__Div float=`left>
            <FC__PageCard.HeaderRow.Title>
              <span
                className=Css.(
                  style([marginRight(`em(0.4)), opacity(0.5)])
                )>
                <FC__Icon.Questionmark isInteractive=false />
              </span>
              {content.headerContent}
            </FC__PageCard.HeaderRow.Title>
          </FC__Div>
        </FC__PageCard.HeaderRow>
        <FC__PageCard.Body>
          <FC__PageCard.BodyPadding v={`em(0.5)}>
            <span
              className=Css.(
                style([
                  color(FC__Settings.Text.LightBackground.p),
                  lineHeight(`em(1.5)),
                ])
              )>
              {content.bodyContent}
            </span>
          </FC__PageCard.BodyPadding>
        </FC__PageCard.Body>
      </FC__PageCard>
    </div>;

  module Jsx2 = {
    let make = (~content, children) =>
      ReasonReactCompat.wrapReactForReasonReact(
        make,
        makeProps(~content, ()),
        children,
      );
  };
};

[@react.component]
let make = (~content) =>
  <FC__Dropdown overlay={<Overlay content />} trigger=FC__Dropdown.Hover>
    <span> <FC__Icon.Questionmark isInteractive=true /> </span>
  </FC__Dropdown>;

module Jsx2 = {
  let make = (~content, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~content, ()),
      children,
    );
};

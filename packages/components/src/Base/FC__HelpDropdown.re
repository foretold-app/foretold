type content = {
  headerContent: string,
  bodyContent: string,
};

module Inside = {
  module Styles = {
    open Css;
    let className = style([maxWidth(`em(20.))]);
  };

  let component = ReasonReact.statelessComponent("HelpDropdown Inside");
  let make = (~content, _children) => {
    ...component,
    render: _self => {
      <div className=Styles.className>
        <FC__PageCard>
          <FC__PageCard.HeaderRow>
            <FC__Div.Jsx2 float=`left>
              <FC__PageCard.HeaderRow.Title>
                <span
                  className=Css.(
                    style([marginRight(`em(0.4)), opacity(0.5)])
                  )>
                  <FC__Icon.Questionmark />
                </span>
                {content.headerContent |> ReasonReact.string}
              </FC__PageCard.HeaderRow.Title>
            </FC__Div.Jsx2>
          </FC__PageCard.HeaderRow>
          <FC__PageCard.Body>
            <FC__PageCard.BodyPadding v={`em(0.0)}>
              <FC__PageCard.P>
                <FC__Markdown.Jsx2 content={content.bodyContent} />
              </FC__PageCard.P>
            </FC__PageCard.BodyPadding>
          </FC__PageCard.Body>
        </FC__PageCard>
      </div>;
    },
  };
};

let component = ReasonReact.statelessComponent("HelpDropdown");

let staticOverlay = content => <Inside content />;

let make = (~content, _children) => {
  ...component,
  render: _self => {
    <FC__Dropdown overlay={staticOverlay(content)} trigger=FC__Dropdown.Hover>
      <span> <FC__Icon.Questionmark /> </span>
    </FC__Dropdown>;
  },
};
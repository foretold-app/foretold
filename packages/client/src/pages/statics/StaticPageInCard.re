open ReactMarkdown;

open Utils;
open E;

let component = ReasonReact.statelessComponent("StaticPageInCard");

let make = (~markdown, _children) => {
  ...component,
  render: _ =>
    <div className=StaticStyles.body>
      <div className=SLayout.Styles.container>
        <div
          className=Css.(style([textAlign(`left), paddingTop(`em(3.))]))>
          <Fc.FC.PageCard>
            <Fc.FC.PageCard.Body>
              <Fc.FC.PageCard.BodyPadding>
                <Markdown source=markdown />
              </Fc.FC.PageCard.BodyPadding>
            </Fc.FC.PageCard.Body>
          </Fc.FC.PageCard>
        </div>
      </div>
    </div>,
};
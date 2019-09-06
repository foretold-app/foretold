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
          <FC.PageCard>
            <FC.PageCard.Body>
              <FC.PageCard.BodyPadding>
                <Markdown source=markdown />
              </FC.PageCard.BodyPadding>
            </FC.PageCard.Body>
          </FC.PageCard>
        </div>
      </div>
    </div>,
};
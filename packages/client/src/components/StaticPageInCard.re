[@react.component]
let make = (~markdown) => {
  <div className=StaticStyles.body>
    <div className={SLayout.Styles.container(`fixedWidth)}>
      <div className=Css.(style([textAlign(`left), paddingTop(`em(3.))]))>
        <ForetoldComponents.PageCard>
          <ForetoldComponents.PageCard.Body>
            <ForetoldComponents.PageCard.BodyPadding>
              <Markdown source=markdown />
            </ForetoldComponents.PageCard.BodyPadding>
          </ForetoldComponents.PageCard.Body>
        </ForetoldComponents.PageCard>
      </div>
    </div>
  </div>;
};
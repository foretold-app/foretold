[@react.component]
let make = (~markdown) => {
  <div className=StaticStyles.body>
    <div className={SLayout.Styles.container(`fixedWidth)}>
      <div className=Css.(style([textAlign(`left), paddingTop(`em(3.))]))>
        <FC.PageCard>
          <FC.PageCard.Body>
            <FC.PageCard.BodyPadding>
              <Markdown source=markdown />
            </FC.PageCard.BodyPadding>
          </FC.PageCard.Body>
        </FC.PageCard>
      </div>
    </div>
  </div>;
};
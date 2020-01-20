[@bs.config {jsx: 3}];

FC.Base.Globals.load();
ReactDOMRe.renderToElementWithId(
  <div className=Css.(style([fontFamily(Settings.Text.standardFont)]))>
    ExampleFullPage.make
  </div>,
  "app",
);

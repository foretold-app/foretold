open FC;
FC.Base.Globals.load();
ReactDOMRe.renderToElementWithId(
  <div className=Css.(style([fontFamily("Lato")]))>
    ExampleFullPage.make
  </div>,
  "app",
);
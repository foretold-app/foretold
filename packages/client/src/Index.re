[@bs.module "./intercom.js"] external intercom: unit => unit = "intercom";

FC.Base.Globals.load();

ReactDOMRe.renderToElementWithId(<App />, "app");

if (Env.prod) {
  intercom();
};

[@bs.module "./intercom.js"]
external intercom: (string, string) => unit = "intercom";

FC.Base.Globals.load();

ReactDOMRe.renderToElementWithId(<App />, "app");

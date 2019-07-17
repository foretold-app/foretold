let instance = AppApolloClient.instance();

FC.Base.Globals.load();

let app =
  <ReasonApollo.Provider client=instance> <App /> </ReasonApollo.Provider>;

ReactDOMRe.renderToElementWithId(app, "app");
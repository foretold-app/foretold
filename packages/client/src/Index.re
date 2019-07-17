let instance = AppApolloClient.instance();

FC.Base.Globals.load();

let app =
  App.make(Layout.make)
  |> ReasonReact.element(~key="app")
  |> E.React.withParent(ReasonApollo.Provider.make(~client=instance));

ReactDOMRe.renderToElementWithId(app, "app");
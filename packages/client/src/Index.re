let instance = AppApolloClient.instance();

FC.Base.Globals.load();

let app =
  App.make(Layout.make, ReasonReact.null)
  |> ReasonReact.element(~key="")
  |> E.React.withParent(ReasonApollo.Provider.make(~client=instance));

ReactDOMRe.renderToElementWithId(app, "app");
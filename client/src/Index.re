let instance = Client.instance();

ReactDOMRe.renderToElementWithId(
  Layout.make
  |> E.React.takeParameterFrom(App.make)
  |> E.React.withParent(ReasonApollo.Provider.make(~client=instance)),
  "app",
);
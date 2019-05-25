let instance = AppApolloClient.instance();

FC.Base.Globals.load();
Layout.make
|> E.React.takeParameterFrom(WithRouteReducer.make)
|> E.React.withParent(ReasonApollo.Provider.make(~client=instance))
|> ReactDOMRe.renderToElementWithId(_, "app");
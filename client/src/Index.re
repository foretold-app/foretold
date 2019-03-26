let instance = ApolloClient.instance();

Layout.make
|> E.React.takeParameterFrom(WithRouteReducer.make)
|> E.React.withParent(ReasonApollo.Provider.make(~client=instance))
|> ReactDOMRe.renderToElementWithId(_, "app");
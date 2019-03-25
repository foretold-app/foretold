let instance = Client.instance();

ReactDOMRe.renderToElementWithId(
  <ReasonApollo.Provider client=instance> <App /> </ReasonApollo.Provider>,
  "app",
);
open FC;

ReactDOMRe.renderToElementWithId(
  <div
    className=Css.(
      style([fontFamily("Lato"), padding2(~v=`em(2.), ~h=`em(2.))])
    )>
    <Lib.Index />
  </div>,
  "main",
);
ReasonReactRouter.push("");

ReactDOMRe.renderToElementWithId(
  <div className=Css.(style([fontFamily("poppins")]))>
    <AppHeader
      links={
        [|
          AppHeader.link(
            ~href="#",
            [|"Public Channels" |> ReasonReact.string|],
          )
          |> ReasonReact.element,
          AppHeader.link(
            ~href="#",
            [|"Entity Explorer" |> ReasonReact.string|],
          )
          |> ReasonReact.element,
        |]
        |> ReasonReact.array
      }
    />
    ExampleFullPage.make
  </div>,
  "app",
);
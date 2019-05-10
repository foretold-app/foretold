ReactDOMRe.renderToElementWithId(
  <div className=Css.(style([fontFamily("poppins")]))>
    <AppHeader
      links={
        [|
          AppHeader.link(~href="#", [|"Link 1" |> ReasonReact.string|])
          |> ReasonReact.element,
          AppHeader.link(~href="#", [|"Link 2" |> ReasonReact.string|])
          |> ReasonReact.element,
        |]
        |> ReasonReact.array
      }
    />
    AppGroupHeader.make
  </div>,
  "app",
);
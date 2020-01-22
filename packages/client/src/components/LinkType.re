type linkType =
  | Action(ReactEvent.Mouse.t => unit)
  | Internal(Routing.Url.t)
  | Relative(string)
  | External(string);

type t = linkType;

let toString = (linkType: t): string =>
  switch (linkType) {
  | Internal(r) => r |> Routing.Url.toString
  | External(s) => s
  | Relative(s) => s
  | Action(_) => "#"
  };

let handleStringUrlClick = (event: ReactEvent.Mouse.t, href) =>
  /* This needed to make sure the page isn't reloaded */
  if (!ReactEvent.Mouse.defaultPrevented(event)) {
    ReactEvent.Mouse.preventDefault(event);
    ReasonReact.Router.push(href);
  };

// @todo: keywords: application state
let onClick = (linkType: t, event) =>
  switch (linkType) {
  | Action(action) => action(event)
  | Internal(_)
  | Relative(_) => handleStringUrlClick(event, linkType |> toString)
  | External(_) => ()
  };

// @todo: keywords: application state
let onClick2 = (linkType: t) =>
  switch (linkType) {
  | Action(_) => ()
  | Internal(_)
  | Relative(_) => linkType |> toString |> ReasonReact.Router.push
  | External(_) => ()
  };
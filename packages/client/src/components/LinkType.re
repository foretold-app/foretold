type linkType =
  | Action(ReactEvent.Mouse.t => unit)
  | Internal(Routing.Url.t)
  | Relative(string)
  | External(string);

type t = linkType;

let toString = (t): string =>
  switch (t) {
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

let onClick = (t: t, event) =>
  switch (t) {
  | Action(action) => action(event)
  | Internal(_) => handleStringUrlClick(event, t |> toString)
  | Relative(_) => handleStringUrlClick(event, t |> toString)
  | External(_) => ()
  };
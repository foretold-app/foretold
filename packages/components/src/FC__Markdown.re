[@bs.module "./FC__Markdown.js"]
external markdown: ReasonReact.reactClass = "default";

[@bs.obj] external makeProps: (~source: string, unit) => _ = "";
let make = (~source, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=markdown,
    ~props=makeProps(),
    children,
  );
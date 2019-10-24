[@bs.config {jsx: 3}];

module Icon = {
  [@bs.module "./Icon.js"]
  external reactClass: ReasonReact.reactClass = "Icon";

  [@react.component]
  let make = (~icon=?, ~size=?, ~children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass,
      ~props={
        "iconType": icon |> E.O.default(""),
        "size": size |> E.O.default("1em"),
      },
      children,
    );
};

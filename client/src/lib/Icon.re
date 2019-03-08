module Icon = {
  [@bs.module "./Icon.js"]
  external reactClass: ReasonReact.reactClass = "Icon";
  let make = (~icon=?, ~size=?, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass,
      ~props={
        "iconType": icon |> Rationale.Option.default(""),
        "size": size |> Rationale.Option.default("1em"),
      },
      children,
    );
};
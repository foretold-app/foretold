module Icon = {
  [@bs.module "./Icon.js"]
  external reactClass: ReasonReact.reactClass = "Icon";
  let make = (~icon=?, ~size=?, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass,
      ~props={
        "iconType": icon |> E.O.default(""),
        "size": size |> E.O.default("1em"),
      },
      children,
    );
};
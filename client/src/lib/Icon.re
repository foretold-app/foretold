/* module Icon = {
     [@bs.module "react-icons-kit/Icon"]
     external reactClass: ReasonReact.reactClass = "Icon";
     [@bs.obj]
     external makeProps:
       (~icon: ReasonReact.reactElement=?, ~size: bool=?, unit) => _ =
       "";
     let make = (~icon=?, ~size=?, children) =>
       ReasonReact.wrapJsForReason(
         ~reactClass,
         ~props=makeProps(~icon?, ~size?, ()),
         children,
       );
   };

   module Home = {
     [@bs.module "react-icons-kit/icomoon/home"]
     external reactClass: ReasonReact.reactClass = "home";
     let make = children =>
       ReasonReact.wrapJsForReason(~reactClass, ~props=(), children);
   }; */

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
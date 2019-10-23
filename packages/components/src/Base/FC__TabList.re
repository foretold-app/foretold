[@bs.config {jsx: 3}];

[@react.component]
let make = (~selected, ~flex=false, ~onClick=?, ~list) =>
  list
  |> FC__E.L.React.fmapi((i, (key, label)) =>
       <FC__Tab
         key={string_of_int(i)}
         onClick={e =>
           switch (onClick) {
           | Some(onClick) =>
             e->ReactEvent.Synthetic.preventDefault;
             onClick(key);
           | None => ()
           }
         }
         isActive={selected == key}
         flex>
         label->React.string
       </FC__Tab>
     );

module Jsx2 = {
  let make = (~selected, ~flex=false, ~onClick=?, ~list, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~selected, ~flex=false, ~onClick?, ~list, ()),
      children,
    );
};

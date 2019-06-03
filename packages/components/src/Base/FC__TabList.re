let component = ReasonReact.statelessComponent(__MODULE__);
let make = (~selected, ~flex=false, ~onClick=?, ~list, _children) => {
  ...component,
  render: _self => {
    list
    |> FC__E.L.React.mapi((i, (key, label)) =>
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
  },
};

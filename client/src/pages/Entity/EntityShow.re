open Utils;

let component = ReasonReact.statelessComponent("EntityShow");

let make = (~id: string, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ => {
    let names = C.Ken.names(id);
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv(id),
      ~body=
        names
        |> E.A.of_list
        |> E.A.fmap((r: Graph_T.T.fact) =>
             <>
               {
                 C.Ken.findName(r.propertyId)
                 |> E.O.default("no-name")
                 |> ste
                 |> E.React.inH3
               }
               Graph_T.T.(
                 switch (r.value.valueType) {
                 | String(s) => s |> ste
                 | ThingId(s) =>
                   <a href={Context.Routing.Url.toString(EntityShow(s))}>
                     {s |> ste}
                   </a>
                 | _ => "no-name" |> ste
                 }
               )
             </>
           )
        |> ReasonReact.array,
    )
    |> layout;
  },
};
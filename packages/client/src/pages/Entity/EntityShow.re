open Utils;

let component = ReasonReact.statelessComponent("EntityShow");

let display = id => {
  let names = C.Ken.names(id);
  names
  |> E.A.of_list
  |> E.A.fmapi((i, r: Graph_T.T.fact) =>
       <div key={i |> string_of_int}>
         {C.Ken.findName(r.propertyId)
          |> E.O.default("no-name")
          |> ste
          |> E.React.inH3}
         Graph_T.T.(
           switch (r.value.valueType) {
           | String(s) => s |> ste
           | ThingId(s) =>
             <Foretold__Components__Link.Jsx2
               linkType={Internal(EntityShow(s))}>
               {s |> ste}
             </Foretold__Components__Link.Jsx2>
           | _ => "no-name" |> ste
           }
         )
       </div>
     )
  |> ReasonReact.array;
};

let make =
    (
      ~pageParams: PageConfig.LoggedInPage.pageParams,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ => {
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv(pageParams.id),
      ~body=
        <FC.PageCard.BodyPadding>
          {display(pageParams.id)}
        </FC.PageCard.BodyPadding>,
    )
    |> layout;
  },
};
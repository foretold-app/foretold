open Utils;

let component = ReasonReact.statelessComponent("EntityShow");

let display = id => {
  <Providers.AppContext.Consumer>
    ...{context => {
      module Config = {
        let globalSetting = context.globalSetting;
      };
      module Ken = KenTools.Functor(Config);
      module MeasurableEntityLinks = MeasurableEntityLinks.Functor(Ken);

      Ken.names(id)
      |> E.A.of_list
      |> E.A.fmapi((i, r: Graph_T.T.fact) =>
           <div key={i |> string_of_int}>
             {Ken.findName(r.propertyId)
              |> E.O.default("no-name")
              |> ste
              |> E.React.inH3}
             Graph_T.T.(
               switch (r.value.valueType) {
               | String(s) => s |> ste
               | ThingId(s) =>
                 <Link.Jsx2 linkType={Internal(EntityShow(s))}>
                   {s |> ste}
                 </Link.Jsx2>
               | _ => "no-name" |> ste
               }
             )
           </div>
         )
      |> ReasonReact.array;
    }}
  </Providers.AppContext.Consumer>;
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
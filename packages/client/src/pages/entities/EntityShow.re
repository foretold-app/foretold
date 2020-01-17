open Utils;

let display = id => {
  <Providers.AppContext.Consumer>
    ...{context => {
      module Config = {
        let entityGraph =
          context.globalSetting
          |> E.O.fmap((e: Types.globalSetting) => e.entityGraph)
          |> E.O.default(None);
      };
      module Ken = KenTools.Functor(Config);
      module MeasurableEntityLinks = MeasurableEntityLinks.Functor(Ken);

      Ken.names(id)
      |> E.A.of_list
      |> E.A.fmapi((i, r: BsKen.Graph_T.T.fact) =>
           <div key={i |> string_of_int}>
             {Ken.findName(r.propertyId)
              |> E.O.default("no-name")
              |> ste
              |> E.React2.inH3}
             BsKen.Graph_T.T.(
               switch (r.value.valueType) {
               | String(s) => s |> ste
               | ThingId(s) =>
                 <Link linkType={Internal(EntityShow(s))}> {s |> ste} </Link>
               | _ => "no-name" |> ste
               }
             )
           </div>
         )
      |> ReasonReact.array;
    }}
  </Providers.AppContext.Consumer>;
};

[@react.component]
let make = (~pageParams: Types.pageParams) => {
  <SLayout head={<SLayout.TextDiv text={pageParams.id} />}>
    <FC.PageCard.BodyPadding>
      {display(pageParams.id)}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};
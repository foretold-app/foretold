[@bs.config {jsx: 3}];

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
      |> E.A.fmapi((i, r: BsKen.Graph_T.T.fact) =>
           <div key={i |> string_of_int}>
             {Ken.findName(r.propertyId)
              |> E.O.default("no-name")
              |> ste
              |> E.React.inH3}
             BsKen.Graph_T.T.(
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

let make = (~pageParams: Types.pageParams, _children) => {
  ...component,
  render: _ => {
    <SLayout head={SLayout.Header.textDiv(pageParams.id)}>
      <FC.PageCard.BodyPadding>
        {display(pageParams.id)}
      </FC.PageCard.BodyPadding>
    </SLayout>;
  },
};

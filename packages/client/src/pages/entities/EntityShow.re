let display = id => {
  let g = KenTools.Graph.fromContext();

  KenTools.Subject.facts(g, id)
  |> E.A.of_list
  |> E.A.fmapi((i, r: KenTools.Fact.fact) =>
       <div key={i |> string_of_int}>
         {KenTools.Subject.name(g, r.propertyId)
          |> E.O.default("no-name")
          |> Utils.ste
          |> E.React2.inH3}
         BsKen.Graph_T.T.(
           switch (r.value.valueType) {
           | String(s) => s |> Utils.ste
           | ThingId(s) =>
             <Link linkType={Internal(EntityShow(s))}>
               {s |> Utils.ste}
             </Link>
           | _ => "no-name" |> Utils.ste
           }
         )
       </div>
     )
  |> ReasonReact.array;
};

[@react.component]
let make = (~pageParams: Types.pageParams) => {
  <SLayout head={<SLayout.TextDiv text={pageParams.id} />}>
    <ForetoldComponents.PageCard.BodyPadding>
      {display(pageParams.id)}
    </ForetoldComponents.PageCard.BodyPadding>
  </SLayout>;
};
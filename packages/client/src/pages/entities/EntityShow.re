let display = id => {
  let g = KenTools.Graph.fromContext();

  <div>
    {KenTools.Subject.facts(g, id)
     |> E.A.of_list
     |> E.A.fmapi((i, r: KenTools.Fact.fact) =>
          <div key={i |> string_of_int}>
            {KenTools.Subject.name(g, r.propertyId)
             |> E.O.bind(_, KenTools.FactValue.toString)
             |> E.O.default("no-name")
             |> Utils.ste
             |> E.React2.inH3}
            KenTools.FactValue.(
              switch (r.value.valueType) {
              | String(s) => s |> Utils.ste
              | ThingId(s) => s |> KenTools.Subject.nameWithLink(g)
              | _ => "no-name" |> Utils.ste
              }
            )
          </div>
        )
     |> ReasonReact.array}
    {KenTools.Subject.isValueToFacts(g, id)
     |> E.A.of_list
     |> E.A.fmapi((i, r: KenTools.Fact.fact) =>
          <div key={i |> string_of_int}>
            {KenTools.Subject.name(g, r.propertyId)
             |> E.O.bind(_, KenTools.FactValue.toString)
             |> E.O.default("no-name")
             |> Utils.ste
             |> E.React2.inH3}
            {r.subjectId |> KenTools.Subject.nameWithLink(g)}
          </div>
        )
     |> ReasonReact.array}
  </div>;
};

[@react.component]
let make = (~pageParams: Types.pageParams) => {
  <SLayout head={<SLayout.TextDiv text={pageParams.id} />}>
    <ForetoldComponents.PageCard.BodyPadding>
      {display(pageParams.id)}
    </ForetoldComponents.PageCard.BodyPadding>
  </SLayout>;
};
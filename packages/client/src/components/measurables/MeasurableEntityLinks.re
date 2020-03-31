let kenDisplay = (g, id) => {
  KenTools.Subject.facts(g, id)
  |> E.A.of_list
  |> E.A.fmapi((i, r: KenTools.Fact.fact) =>
       <div key={i |> string_of_int}>
         {KenTools.Subject.name(g, r.propertyId)
          |> E.O.bind(_, KenTools.FactValue.toString)
          |> E.O.default("no-name")
          |> Utils.ste
          |> E.React2.inH3}
         {switch (r.value.valueType) {
          | String(s) => s |> Utils.ste
          | JSON(_s) => "" |> Utils.ste
          | ThingId(s) =>
            <Link linkType={Internal(EntityShow(s))}>
              {s |> Utils.ste}
            </Link>
          }}
       </div>
     )
  |> ReasonReact.array;
};

let showAttribute = (~attribute: option(string), ~g, ~className) =>
  attribute
  |> E.O.bind(_, KenTools.Subject.name(g))
  |> E.O.bind(_, KenTools.FactValue.toString)
  |> E.O.bind(_, r =>
       attribute
       |> E.O.fmap(_d =>
            <Antd_Popover
              content={kenDisplay(g, attribute |> E.O.default(""))}
              trigger=`hover
              placement=`top>
              <span className> {r |> Utils.ste} </span>
            </Antd_Popover>
          )
     );

let xEntityLink = (attribute, ~g, ~m: Types.measurable, ~className: string) =>
  showAttribute(~attribute=attribute(m), ~g, ~className);

let nameEntityLink = xEntityLink(r => r.labelSubject);
let propertyEntityLink = xEntityLink(r => r.labelProperty);
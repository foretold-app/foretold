// @todo: To make a component.
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

// @todo: To make a component.
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

let xEntityLinks = (attribute, ~g, ~m: Types.measurable, ~className: string) => {
  switch (attribute(m)) {
  | Some(attributes) =>
    attributes
    |> E.A.fmap(attribute => {
         showAttribute(~attribute=Some(attribute), ~g, ~className)
         |> E.O.default(<Null />)
       })
    |> ReasonReact.array
  | None => <Null />
  };
};

let nameEntityLink = xEntityLink(r => r.labelSubject);
let propertyEntityLink = xEntityLink(r => r.labelProperty);
let conditionalOnEntityLinks = xEntityLinks(r => r.labelConditionals);
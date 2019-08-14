let kenDisplay = id => {
  let names = Foretold__Components__Ken.names(id);
  names
  |> E.A.of_list
  |> E.A.fmapi((i, r: Graph_T.T.fact) =>
       <div key={i |> string_of_int}>
         {Foretold__Components__Ken.findName(r.propertyId)
          |> E.O.default("no-name")
          |> Utils.ste
          |> E.React.inH3}
         Graph_T.T.(
           switch (r.value.valueType) {
           | String(s) => s |> Utils.ste
           | ThingId(s) =>
             <Link.Jsx2 linkType={Internal(EntityShow(s))}>
               {s |> Utils.ste}
             </Link.Jsx2>
           | _ => "no-name" |> Utils.ste
           }
         )
       </div>
     )
  |> ReasonReact.array;
};

let xEntityLink = (attribute, ~m: Types.measurable, ~className: string) =>
  m
  |> attribute
  |> E.O.bind(_, Foretold__Components__Ken.findName)
  |> E.O.bind(_, r =>
       m
       |> attribute
       |> E.O.fmap(d =>
            <AntdPopover
              content={kenDisplay(attribute(m) |> E.O.default(""))}
              trigger=`hover
              placement=`top>
              <span className> {r |> Utils.ste} </span>
            </AntdPopover>
          )
     );

let nameEntityLink = xEntityLink(r => r.labelSubject);
let propertyEntityLink = xEntityLink(r => r.labelProperty);
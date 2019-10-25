[@bs.config {jsx: 3}];

module Functor = (Ken: KenTools.KenModule) => {
  let kenDisplay = id => {
    Ken.names(id)
    |> E.A.of_list
    |> E.A.fmapi((i, r: BsKen.Graph_T.T.fact) =>
         <div key={i |> string_of_int}>
           {Ken.findName(r.propertyId)
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

  // @todo: 1

  let xEntityLink = (attribute, ~m: Types.measurable, ~className: string) =>
    m
    |> attribute
    |> E.O.bind(_, Ken.findName)
    |> E.O.bind(_, r => m |> attribute |> E.O.fmap(_d => ReasonReact.null));

  let nameEntityLink = xEntityLink(r => r.labelSubject);
  let propertyEntityLink = xEntityLink(r => r.labelProperty);
};

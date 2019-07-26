type measurable = Types.measurable;
let graph = Data.make;

let itemUrl = id => Routing.Url.toString(EntityShow(id));

let findName = propertyId =>
  graph
  |> Graph_T.F.factList
  |> Graph_Fact_Filters.withSubject(propertyId)
  |> Graph_Fact_Filters.withProperty("@base/properties/p-name")
  |> E.L.head
  |> E.O.bind(_, (k: Graph_T.T.fact) =>
       switch (k.value.valueType) {
       | String(s) => Some(s)
       | ThingId(s) => Some(s)
       | _ => None
       }
     );

let findInstanceOfName = propertyId =>
  graph
  |> Graph_T.F.factList
  |> Graph_Fact_Filters.withSubject(propertyId)
  |> Graph_Fact_Filters.withProperty("@base/properties/p-instance-of")
  |> E.L.head
  |> E.O.bind(_, (k: Graph_T.T.fact) =>
       switch (k.value.valueType) {
       | String(s) => Some(s)
       | ThingId(s) => findName(s)
       | _ => None
       }
     );

let names = subjectId =>
  graph
  |> Graph_T.F.factList
  |> Graph_Fact_Filters.withSubject(subjectId)
  |> E.L.fmap((f: Graph_T.T.fact) => f);

let things = graph |> Graph_T.F.thingArray;
open BsKen;

module type KenModule = {
  type t = Graph_T.T.thing;
  let graph: Graph_Dirs.t;
  let itemUrl: string => string;
  let findName: string => option(string);
  let names: string => list(Graph_T.T.fact);
  let subjectIdToDisplay: string => ReasonReact.reactElement;
  let findInstanceOfName: string => option(Graph_T.T.thingIdString);
  let things: array(t);
  let getName: t => string;
  let getInstanceOfName: t => string;
  let withNames: Js.Array.t(t) => Js.Array.t(t);
  let dataSource: Js.Array.t(t);
};

module Functor = (Config: {let entityGraph: option(Js.Json.t);}) : KenModule => {
  type t = Graph_T.T.thing;

  let graph =
    Config.entityGraph
    |> E.O.default(Js.Json.array([||]))
    |> (e => Interface.Graph.fromJson(e));

  let itemUrl = entityId => Routing.Url.toString(EntityShow(entityId));

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

  let getName = t => t |> Graph_T.Thing.id |> findName |> E.O.default("");

  let getInstanceOfName = t =>
    t |> Graph_T.Thing.id |> findInstanceOfName |> E.O.default("");

  let hasName = t => getName(t) != "";

  let withNames = E.A.filter(hasName);

  let dataSource = things |> withNames;

  let subjectIdToDisplay = subjectId => {
    subjectId
    |> names
    |> E.A.of_list
    |> E.A.fmapi((i, r: BsKen.Graph_T.T.fact) =>
         <div key={i |> string_of_int}>
           {findName(r.propertyId)
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
};
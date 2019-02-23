let uniqueIds = (facts: SimpleFactList_T.graph): list(SimpleFactList_T.id) =>
  facts
  |> List.map((e: SimpleFactList_T.fact) =>
       [e.id, e.subjectId, e.propertyId]
     )
  |> List.flatten
  |> Rationale.RList.uniqBy((a: SimpleFactList_T.id) => a.id);

let findType =
    (id: SimpleFactList_T.id, facts: SimpleFactList_T.graph)
    : Graph_T.T.thingTypes =>
  switch (id) {
  | {id}
      when facts |> List.exists((f: SimpleFactList_T.fact) => f.id.id == id) =>
    FACT
  | _ => ITEM
  };

let listFacts = (graph: SimpleFactList_T.graph): list(Graph_T.T.fact) =>
  graph
  |> List.map((f: SimpleFactList_T.fact) =>
       (
         {
           thingIdString: f.id.id,
           subjectId: f.subjectId.id,
           propertyId: f.propertyId.id,
           value: f.value,
         }: Graph_T.T.fact
       )
     );

/* TODO: Possibly delete. */
let possiblyConvertValueTypesToThing =
    (graph: Graph_T.T.t, value: Graph_T.T.value) =>
  switch (value.valueType) {
  | String(s) =>
    graph.things |> Js.Dict.get(_, s) |> Rationale.Option.isSome ?
      Graph_T.T.ThingId(s) : Graph_T.T.String(s)
  | _ => value.valueType
  };

/* TODO: Delete this, after making sure */
let connectValuesToFacts = (graph: Graph_T.T.t): Graph_T.T.t => {
  ...graph,
  facts:
    graph.facts
    |> Js.Dict.values
    |> Array.map((f: Graph_T.T.fact) =>
         {
           ...f,
           value: {
             valueType: possiblyConvertValueTypesToThing(graph, f.value),
           },
         }
       )
    |> Array.to_list
    |> List.map((r: Graph_T.T.fact) => (r.thingIdString, r))
    |> Js.Dict.fromList,
};

let listThings = (facts: SimpleFactList_T.graph): list(Graph_T.T.thing) =>
  facts
  |> uniqueIds
  |> List.map((id: SimpleFactList_T.id) =>
       (
         {
           thingId: {
             thingIdString: id.id,
             baseId: "FIXIME TODO",
             isPublic: id.isPublic,
           },
           thingType: Graph_T.T.Item,
         }: Graph_T.T.thing
       )
     );

let run = (facts: SimpleFactList_T.graph): Graph_T.T.t =>
  Graph_T.T.(
    {
      bases:
        facts
        |> listThings
        |> List.map((r: Graph_T.T.thing) => r.thingId.baseId)
        |> Rationale.RList.uniq,
      things:
        facts
        |> listThings
        |> List.map((r: Graph_T.T.thing) => (r.thingId.thingIdString, r))
        |> Js.Dict.fromList,
      facts:
        facts
        |> listFacts
        |> List.map((r: Graph_T.T.fact) => (r.thingIdString, r))
        |> Js.Dict.fromList,
      directories: [],
    }
    |> Graph_Dirs.makeDirs
  );
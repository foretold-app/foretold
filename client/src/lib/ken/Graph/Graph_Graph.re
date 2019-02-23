open Rationale.Function.Infix;
open Graph_T.T;

type t = Graph_T.T.t;

let findFact = id => Graph_T.F.facts ||> (e => Js.Dict.get(e, id));
let findThing = (id: string, g: t): option(Graph_T.T.thing) =>
  g |> Graph_T.F.findThing(id);
let findThingFromFact = (g: t, edge: edge, f: fact) =>
  f |> Graph_Fact.edgeId(edge) |> (id => Graph_T.F.findThing(id, g));
let from_json = Json.Decode.list(Graph_Fact.from_json);
let to_json = (t: t) => {
  let facts = t.facts |> Js.Dict.values |> Array.map(Graph_Fact.to_json);
  let things = t.things |> Js.Dict.values |> Array.map(Graph_T.Thing.to_json);
  Json.Encode.(
    object_([
      (Config.JsonKeys.facts, jsonArray(facts)),
      (Config.JsonKeys.things, jsonArray(things)),
    ])
  );
};
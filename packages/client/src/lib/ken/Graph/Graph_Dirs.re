open Rationale.Function.Infix;
open Graph_T.T;
open Graph_T;

type t = Graph_T.T.t;

let bases = t => t.bases;

let makeDirs = (t: t): t => {
  let allThingIdStrings =
    t.things
    |> Js.Dict.values
    |> Array.to_list
    |> E.L.fmap(e => e.thingId.thingIdString);

  let possibleUniqueIds =
    E.L.fmap(Directory.parent)
    ||> E.L.filter(e => !Directory.isFactDirectory(e))
    ||> E.L.fmap(Directory.allSubdirectories)
    ||> E.L.concat
    ||> E.L.uniq;

  let directories = allThingIdStrings |> possibleUniqueIds;

  {...t, directories};
};
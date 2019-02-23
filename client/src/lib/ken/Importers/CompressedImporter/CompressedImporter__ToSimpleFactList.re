type unprocessedGraph = CompressedImporter__T.graph;

let valueToArray = value =>
  switch (value) {
  | CompressedImporter__T.String(str) => [|str|]
  | CompressedImporter__T.Array(strs) => strs
  };

/* This makes sure that values that are lists become single values. */
let flattenValues = (g: unprocessedGraph): unprocessedGraph =>
  CompressedImporter__T.(
    g
    |> Array.map((package: CompressedImporter__T.package) =>
         {
           ...package,
           things:
             package.things
             |> Array.map(r =>
                  {
                    ...r,
                    facts:
                      r.facts
                      |> Array.map((f: fact) =>
                           f.value
                           |> valueToArray
                           |> Array.map(value =>
                                {...f, value: String(value)}
                              )
                         )
                      |> Belt.Array.concatMany,
                  }
                ),
         }
       )
  );

let allPackageFacts = (p: CompressedImporter__T.package) =>
  p.things
  |> Array.map((thing: CompressedImporter__T.thing) =>
       thing.facts
       |> Array.map((fact: CompressedImporter__T.fact) => {
            let makeId = Compiler_Run.makeThingId;
            (
              {
                thingId: makeId(fact.id),
                subjectId: makeId(Some(thing.id)),
                propertyId: makeId(Some(fact.property)),
                isInversed: fact.isInversed,
                value:
                  Compiler_AST.String(
                    switch (fact.value) {
                    | String(str) => str
                    | _ => "ERROR"
                    },
                  ),
              }: Compiler_AST.fact
            );
          })
     )
  |> Belt.Array.concatMany
  |> Array.to_list;

let formattedAliases =
    (d: Js.Dict.t(string)): Js.Dict.t(Compiler_AST.thingId) =>
  d
  |> Js.Dict.entries
  |> Array.map(((id, v)) =>
       (
         (
           id,
           {
             rawId: Some(v),
             tag: None,
             thingIdType: Some(Compiler_AST.NONFACT),
             updatedId: Some(v),
           },
         ): (
           string,
           Compiler_AST.thingId,
         )
       )
     )
  |> Js.Dict.fromArray;

/* This doesn't work, not sure why */
/* let formattedAliases2 =
     (d: Js.Dict.t(string)): Js.Dict.t(Compiler_AST.thingId) =>
   d
   |> Js.Dict.map((. id) =>
        (
          {
            rawId: Js.Dict.get(d, id),
            tag: None,
            thingIdType: Some(Compiler_AST.NONFACT),
            updatedId: Js.Dict.get(d, id),
          }: Compiler_AST.thingId
        )
      ); */

let shape = (g: unprocessedGraph): list(Compiler_AST.package) =>
  g
  |> Array.map((package: CompressedImporter__T.package) =>
       (
         {
           facts: package |> allPackageFacts,
           baseId: package.baseId,
           resourceId: package.resourceId,
           aliases: package.aliases |> formattedAliases,
         }: Compiler_AST.package
       )
     )
  |> Array.to_list;

let combinePackages =
    (packages: list(Compiler_AST.package)): SimpleFactList_T.graph =>
  Rationale.Function.Infix.(
    packages
    |> List.map(Compiler_Run.run ||> Compiler_Run.toSimple)
    |> SimpleFactList_T.combine
  );

let run =
  Rationale.Function.Infix.(flattenValues ||> shape ||> combinePackages);
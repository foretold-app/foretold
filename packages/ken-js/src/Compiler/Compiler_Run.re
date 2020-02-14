open Compiler_AST;

let makeThingId = id => {
  rawId: id,
  updatedId: None,
  thingIdType: None,
  tag: None,
};

let thingIdKey = (e: thingId) => (e.rawId, e.tag);

let allPrimaryIds = (g: package): list(thingId) => {
  let factIds =
    g.facts
    |> List.map(r => [r.thingId, r.subjectId, r.propertyId])
    |> List.flatten;
  let aliasIds = g.aliases |> Js.Dict.values |> Array.to_list;
  let all = factIds |> List.append(aliasIds);
  all;
};

let findUniqueIds = (g: package): list(thingId) =>
  g |> allPrimaryIds |> Rationale.RList.uniqBy(thingIdKey);

/* Make sure that all thing Ids are only represented once. */
/* Don't do this for facts! */
let tagFacts = (g): package => {
  g.facts
  |> List.iter(fact => {
       Random.self_init();
       let nonce = Random.int(1000000) |> string_of_int;
       fact.thingId.tag = Some(nonce);
     });
  g;
};

let useUniqueThingIds = (g): package => {
  let uniqueIds = findUniqueIds(g);
  let findId = thingId =>
    uniqueIds |> List.find(e => thingIdKey(e) == thingIdKey(thingId));
  let facts =
    g.facts
    |> List.map(r =>
         {
           ...r,
           thingId: findId(r.thingId),
           subjectId: findId(r.subjectId),
           propertyId: findId(r.propertyId),
           value:
             switch (r.value) {
             | Id(thingId) => Id(findId(thingId))
             | _ => r.value
             },
         }
       );
  let aliases =
    g.aliases
    |> Js.Dict.entries
    |> Array.map(((id: string, v: thingId)) => (id, findId(v)))
    |> Js.Dict.fromArray;
  {...g, facts, aliases};
};

/* Mutate thing types to correct formats */
let handleThingTypes = (g: package) => {
  let propertyOrSubjectType = (id: thingId) =>
    switch (id.thingIdType) {
    | Some(FACT) => Some(FACT)
    | _ => Some(NONFACT)
    };
  g.facts
  |> List.iter(r => {
       let id = r.thingId;
       id.thingIdType = Some(FACT);
     });
  g.facts
  |> List.iter(r => {
       let propertyId = r.propertyId;
       propertyId.thingIdType = propertyOrSubjectType(propertyId);
       let subjectId = r.subjectId;
       subjectId.thingIdType = propertyOrSubjectType(subjectId);
     });
  g;
};

let findId = (uniqueIds, thingId) =>
  uniqueIds |> List.find(e => thingIdKey(e) == thingIdKey(thingId));

let _convertValue = (package, uniqueIds, fact) =>
  switch (fact.value) {
  | Id(id) => Id(id)
  | String(str) =>
    let alias = package.aliases |> Js.Dict.get(_, str);
    switch (alias) {
    | Some(s) => Id(s)
    | _ =>
      uniqueIds
      |> Belt.List.getBy(_, e => thingIdKey(e) == (Some(str), None))
      |> (
        e =>
          switch (e) {
          | Some(id) => Id(id)
          | _ => String(str)
          }
      )
    };
  };

let linkValues = (p): package => {
  let uniqueIds = findUniqueIds(p);
  p.facts |> List.iter(fact => fact.value = _convertValue(p, uniqueIds, fact));
  p;
};

let convertIdd = (package: Compiler_AST.package, thingId) => {
  open Rationale.Option;
  /* let rawId = thingId.rawId |> default("CHANGE_ME_SHOULD_BE_RANDOM"); */
  let alias =
    package.aliases |> Js.Dict.get(_, thingId.rawId |> default(""));
  switch (thingId.rawId) {
  | Some(r) =>
    switch (alias) {
    | Some({rawId: Some(a)}) => Some(a)
    | _ when IdConverter.isFullId(r) => Some(r)
    | _ => IdConverter.toFullId(package.baseId, package.resourceId, r)
    }
  | _ => None
  };
};

let generateFactId = (thingId, subjectId, package: package) => {
  open Rationale.Option;
  let subject =
    subjectId.updatedId
    |> toExn(
         "Subject ThingID expected to have updatedID by this point of pipeline",
       );
  let isInSameBase = Graph_T.Directory.root(subject) == package.baseId;
  let tagId =
    thingId.tag |> toExn("Expected fact to have fact ID at this point.");
  isInSameBase ?
    subject ++ "/_f/" ++ tagId :
    IdConverter.toFullId(package.baseId, package.resourceId, "_f/" ++ tagId)
    |> toExn("Expect Full Base Id Id");
};

let handleUpdatedIds = (p): package => {
  let uniqueIds = findUniqueIds(p);
  uniqueIds
  |> List.iter(id =>
       switch (id.thingIdType) {
       | Some(NONFACT) => id.updatedId = convertIdd(p, id)
       | _ => ()
       }
     );

  p.facts
  |> List.iter(fact =>
       fact.thingId.updatedId =
         Some(generateFactId(fact.thingId, fact.subjectId, p))
     );
  p;
};

let showFacts = (g: package) =>
  g.facts |> Array.of_list |> Array.map(factToJs);
let showIds = (g: package) =>
  g |> findUniqueIds |> Array.of_list |> Array.map(thingIdToJs);

let inverseFact = fact => {
  let inversed =
    switch (fact.value) {
    | Id(f) => {
        ...fact,
        isInversed: false,
        value: Id(fact.subjectId),
        subjectId: f,
      }
    | String(r) => {
        ...fact,
        isInversed: false,
        value: Id(fact.subjectId),
        subjectId: {
          rawId: Some(r),
          tag: None,
          thingIdType: None,
          updatedId: None,
        },
      }
    };
  inversed;
};

let handleInverseFacts = (package: package) => {
  ...package,
  facts: package.facts |> List.map(f => f.isInversed ? inverseFact(f) : f),
};

let run =
  Rationale.Function.Infix.(
    handleInverseFacts
    ||> tagFacts
    ||> useUniqueThingIds
    ||> handleThingTypes
    ||> linkValues
    ||> handleUpdatedIds
  );

let convertId = (f: Compiler_AST.thingId): SimpleFactList_T.id => {
  id: f.updatedId |> Rationale.Option.toExn(""),
  isPublic: false,
};

let toSimple = (g: Compiler_AST.package): SimpleFactList_T.graph =>
  g.facts
  |> List.map((f: Compiler_AST.fact) =>
       (
         {
           id: convertId(f.thingId),
           subjectId: convertId(f.subjectId),
           propertyId: convertId(f.propertyId),
           value: {
             valueType:
               switch (f.value) {
               | String(str) => Graph_T.T.String(str)
               | Id(id) =>
                 Graph_T.T.ThingId(
                   id.updatedId
                   |> Rationale.Option.toExn(
                        "Error: thingId does not have #updatedId when needed: "
                        ++ (id.rawId |> Rationale.Option.default("")),
                      ),
                 )
               },
           },
         }: SimpleFactList_T.fact
       )
     );

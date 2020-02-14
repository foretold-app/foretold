open Rationale.Function.Infix;

module T = {
  type thingIdString = string;

  type baseId = string;

  type valueType =
    | String(string)
    | ThingId(thingIdString)
    | JSON(Js.Json.t);

  [@bs.deriving jsConverter]
  type value = {valueType};

  [@bs.deriving jsConverter]
  type fact = {
    thingIdString,
    subjectId: thingIdString,
    propertyId: thingIdString,
    value,
  };

  type thingTypes =
    | FACT
    | BASE
    | ITEM;

  type thingType =
    | Fact(fact)
    | Item;

  type thingId = {
    thingIdString,
    isPublic: bool,
    baseId,
  };

  [@bs.deriving jsConverter]
  type thing = {
    thingId,
    thingType,
  };

  type t = {
    things: Js.Dict.t(thing),
    facts: Js.Dict.t(fact),
    bases: list(thingIdString),
    directories: list(string),
  };

  type things = Js.Dict.t(thing);
  type facts = Js.Dict.t(fact);

  [@genType]
  type edge =
    | SUBJECT
    | PROPERTY
    | VALUE;
};

let listCombinations: list('a) => list(list('a)) =
  Utility.accumulator(~accum=[], ~history=[], (accum, head) =>
    List.append(accum, [head])
  );

module Directory = {
  type t = string;
  let from_array = Js.Array.joinWith("/");
  let to_array = Js.String.split("/");
  let to_list = to_array ||> Array.to_list;
  let from_list = Array.of_list ||> from_array;
  let isRoot = e => e |> to_array |> Array.length == 1;
  let root = e => e |> to_array |> Array.get(_, 0);
  let isFactDirectory = e =>
    e
    |> to_list
    |> (
      e => {
        Rationale.RList.last(e) == Some("_f");
      }
    );
  let allSubdirectories =
    to_list ||> listCombinations ||> List.map(from_list);

  let removeLastNDirs = n =>
    to_list ||> Rationale.RList.dropLast(n) ||> from_list;

  let parent = removeLastNDirs(1);
};

module F = {
  let things = (g: T.t) => g.things;
  let thingArray = (g: T.t) => g.things |> Js.Dict.values;
  let findThing = (id: T.thingIdString, t: T.t) =>
    Js.Dict.get(t |> things, id);

  let facts = (g: T.t) => g.facts;
  let factArray = facts ||> Js.Dict.values;
  let factList = facts ||> Js.Dict.values ||> Array.to_list;

  let directories = (g: T.t) => g.directories;

  let rootDirectories = (g: T.t) =>
    g.directories |> List.filter(Directory.isRoot);

  let childDirectories = (g: T.t, s: string) =>
    g.directories |> List.filter(e => Directory.parent(e) == s);

  let factsJs = (g: T.t) =>
    g.facts |> Js.Dict.values |> Array.map(T.factToJs);

  let thingsJs = (g: T.t) =>
    g.things |> Js.Dict.values |> Array.map(T.thingToJs);

  let valuesJs = (g: T.t) =>
    g.facts
    |> Js.Dict.values
    |> Array.map((f: T.fact) => f.value)
    |> Array.map(T.valueToJs);
};

module Thing = {
  open T;
  type t = T.thing;
  let id = e => e.thingId.thingIdString;

  [@genType]
  let to_s = e => "[ID: " ++ (e |> id) ++ "]";

  [@genType]
  let to_json = (t: t) => Json.Encode.(object_([("id", string(id(t)))]));
};

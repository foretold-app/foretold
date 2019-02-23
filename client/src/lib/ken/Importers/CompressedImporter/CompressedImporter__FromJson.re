open CompressedImporter__T;

let getPropertyId = p => {
  let isInversed = p.[0] == {|\|}.[0];
  isInversed ? (true, Js.String.sliceToEnd(~from=1, p)) : (false, p);
};

let factDecoder = (propertyId, json) => {
  let (isInversed, property) = getPropertyId(propertyId);
  switch (json |> Js.Json.classify) {
  | JSONString(_) => {
      value: String(json |> Json.Decode.string),
      property,
      isInversed,
      id: None,
    }
  | JSONObject(_) => {
      value: String(json |> Json.Decode.field("value", Json.Decode.string)),
      property,
      isInversed,
      id: None,
    }
  | JSONArray(rs) => {
      value: Array(rs |> Array.map(Json.Decode.string)),
      property,
      isInversed,
      id: None,
    }
  | _ => {
      property,
      isInversed: false,
      id: None,
      value: String("Couldn't find"),
    }
  };
};

let filterArray = (filter, ar) =>
  ar |> Array.to_list |> filter |> Array.of_list;

let filteredFactKeys = ["config"];
let propertyDecoder = json => {
  let thing0 =
    Js.Json.decodeObject(json) |> Rationale.Option.toExn("Parse Error");

  let toFact = id => {
    let _value =
      Js.Dict.get(thing0, id) |> Rationale.Option.toExn("Parse Error");
    factDecoder(id, _value);
  };

  let nonTemplateKeys =
    thing0
    |> Js.Dict.keys
    |> filterArray(Rationale.RList.without(filteredFactKeys));

  let facts = nonTemplateKeys |> Array.map(toFact);
  facts;
};

let removeIfInList = (list, fn) =>
  List.filter(e => e |> fn |> Rationale.RList.contains(_, list) |> (e => !e));

let decodeBase = json => {
  let entries =
    json
    |> Js.Json.decodeObject
    |> Rationale.Option.toExn("Parse Error")
    |> Js.Dict.entries
    |> Array.to_list;
  open Json.Decode;
  let baseId = json |> field("config", field("baseId", string));
  let resourceId = json |> field("config", field("resourceId", string));
  let aliases =
    json |> field("config", field("aliases", Json.Decode.dict(string)));
  let things =
    entries
    |> removeIfInList(filteredFactKeys, ((k, _)) => k)
    |> List.map(((key, value)) => {id: key, facts: propertyDecoder(value)});
  {things: things |> Array.of_list, baseId, resourceId, aliases};
};

let run = json: graph => json |> Json.Decode.array(decodeBase);
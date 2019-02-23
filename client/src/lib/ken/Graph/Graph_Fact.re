open Graph_T.T;
open Config;

type t = fact;
let subjectId = t => t.subjectId;
let propertyId = t => t.propertyId;
let edgeId = edge => edge == SUBJECT ? subjectId : propertyId;
let value = t => t.value;
let id = (t: t) => t.thingIdString;
let to_json = (t: t) =>
  Json.Encode.(
    object_([
      (FactJson.Fields.id, string(t.thingIdString)),
      (FactJson.Fields.subjectId, string(t.subjectId)),
      (FactJson.Fields.propertyId, string(t.propertyId)),
      (FactJson.Fields.value, Graph_Value.to_json(t.value)),
    ])
  );

let from_json = (t: Js.Json.t) =>
  Json.Decode.{
    thingIdString: t |> field(FactJson.Fields.id, string),
    subjectId: t |> field(FactJson.Fields.subjectId, string),
    propertyId: t |> field(FactJson.Fields.propertyId, string),
    value: {
      valueType: t |> field(FactJson.Fields.value, Graph_Value.from_json),
    },
  };
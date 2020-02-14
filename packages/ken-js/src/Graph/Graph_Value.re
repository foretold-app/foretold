open Graph_T.T;
open Config;

open FactJson.Value;
let to_json = (v: value) =>
  Json.Encode.(
    switch (v.valueType) {
    | ThingId(s) =>
      object_([
        (dataTypeField, string(thingIdType)),
        (dataField, string(s)),
      ])
    | String(s) =>
      object_([
        (dataTypeField, string(stringType)),
        (dataField, string(s)),
      ])
    | JSON(s) =>
      object_([(dataTypeField, string(jsonType)), (dataField, s)])
    }
  );

let from_json = (v: Js.Json.t) => {
  open Json.Decode;
  let _type = v |> field(dataTypeField, string);
  switch (_type) {
  | "string" => v |> field(dataField, string) |> (e => String(e))
  | "thingId" => v |> field(dataField, string) |> (e => ThingId(e))
  | _ => v |> field(dataField, string) |> (e => ThingId(e))
  };
};
type error = string;

type jwt = Js_dict.t(Js.Json.t);

[@bs.module] external decode: string => Js.Json.t = "jwt-decode";

let get_attribute = (attribute: string, jwt: jwt) =>
  Js.Dict.get(jwt, attribute) |> E.O.bind(_, Js.Json.decodeString);

let make = (s: string) => s |> decode |> Js.Json.decodeObject;
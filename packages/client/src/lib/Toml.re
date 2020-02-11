[@bs.module "@iarna/toml"] external parse: string => Js.Json.t = "parse";

let realParse = string =>
  try (Belt.Result.Ok(parse(string))) {
  | Js.Exn.Error(e) =>
    switch (Js.Exn.message(e)) {
    | Some(message) => Error(message)
    | None => Error("Unknown error")
    }
  };
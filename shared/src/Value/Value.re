type trio = {
  p25: float,
  p50: float,
  p75: float,
};

type point = float;

type value =
  | Trio(trio)
  | Point(point);

let parseTrio = json =>
  Json.Decode.{
    p25: json |> field("p25", Json.Decode.float),
    p50: json |> field("p50", Json.Decode.float),
    p75: json |> field("p75", Json.Decode.float),
  };

let encodeTrio: trio => Js.Json.t =
  (trio: trio) =>
    Json.Encode.(
      object_([
        ("p25", Json.Encode.float(trio.p25)),
        ("p50", Json.Encode.float(trio.p50)),
        ("p75", Json.Encode.float(trio.p75)),
      ])
    );

let parseValue = json => {
  let trio = json |> Json.Decode.(optional(field("trio", parseTrio)));
  let point =
    json |> Json.Decode.(optional(field("point", Json.Decode.float)));
  switch (trio, point) {
  | (Some(t), None) => Trio(t)
  | (None, Some(t)) => Point(t)
  | _ => Trio({p25: 0.0, p50: 0.0, p75: 0.0})
  };
};

let encodeValue = (value: value) =>
  switch (value) {
  | Trio(t) => Json.Encode.(object_([("trio", encodeTrio(t))]))
  | Point(p) => Js.Json.string("")
  };
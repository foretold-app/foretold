type trio = {
  p25: float,
  p50: float,
  p75: float,
};

type value = {
  trio: option(trio),
  pointEstimate: option(float),
};

let parseTrio = json =>
  Json.Decode.{
    p25: json |> field("p25", float),
    p50: json |> field("p50", float),
    p75: json |> field("p75", float),
  };

let parseValue = json =>
  Json.Decode.{
    trio: json |> optional(field("trio", parseTrio)),
    pointEstimate: json |> optional(field("pointEstimate", float)),
  };

let encodeTrio = (trio: trio) =>
  Json.Encode.(
    object_([
      ("p25", float(trio.p25)),
      ("p50", float(trio.p50)),
      ("p75", float(trio.p75)),
    ])
  );

let encodeValue = (value: value) =>
  switch (value.trio) {
  | Some(t) => Json.Encode.(object_([("trio", encodeTrio(t))]))
  | None => Js.Json.string("")
  };

type competitorType = [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE];
type measurement = {
  .
  "agent":
    option(
      {
        .
        "bot":
          option(
            {
              .
              "id": string,
              "name": option(string),
              "competitorType": competitorType,
            },
          ),
        "id": string,
        "user":
          option(
            {
              .
              "id": string,
              "name": string,
            },
          ),
      },
    ),
  "relevantAt": MomentRe.Moment.t,
  "competitorType": competitorType,
  "createdAt": MomentRe.Moment.t,
  "taggedMeasurementId": option(string),
  "value": value,
};
type measurements = Js.Array.t(option(measurement));
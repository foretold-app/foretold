type competitorType = [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE];
type measurement = {
  .
  "id": string,
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
        "name": option(string),
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
  "description": option(string),
  "relevantAt": MomentRe.Moment.t,
  "competitorType": competitorType,
  "createdAt": MomentRe.Moment.t,
  "taggedMeasurementId": option(string),
  "value": Belt.Result.t(Value.t, string),
};
type measurements = Js.Array.t(option(measurement));
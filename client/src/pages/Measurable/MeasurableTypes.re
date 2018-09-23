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
  "value": Belt.Result.t(Shared.Value.t, string),
};
type measurements = Js.Array.t(option(measurement));
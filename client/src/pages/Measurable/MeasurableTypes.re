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
            },
          ),
        "id": string,
        "type": [ | `BOT | `USER],
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
  "createdAt": MomentRe.Moment.t,
  "percentile25": string,
  "percentile50": string,
  "percentile75": string,
};
type measurements = Js.Array.t(option(measurement));
/* let agentName = optionInternal(b => b##name); */
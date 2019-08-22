type state = [ | `OPEN | `JUDGEMENT_PENDING | `JUDGED | `CLOSED_AS_UNRESOLVED];

type query = {state: option(state)};

let make = (state: option(state)): query => {state: state};

let stateFromString = (r: string): option(state) =>
  switch (r) {
  | "open" => Some(`OPEN)
  | "closed" => Some(`JUDGED)
  | "pending" => Some(`JUDGEMENT_PENDING)
  | "closedAsUnresolved" => Some(`CLOSED_AS_UNRESOLVED)
  | _ => None
  };

let statetoString = (s: state) =>
  switch (s) {
  | `OPEN => "open"
  | `JUDGED => "closed"
  | `JUDGEMENT_PENDING => "pending"
  | `CLOSED_AS_UNRESOLVED => "closedAsUnresolved"
  };

let fromString = (r: string): query => {
  let keyValuePairs = KeyValuePairs.fromSearchParam(r);
  let state =
    switch (keyValuePairs |> KeyValuePairs.get("state")) {
    | Some(state) => stateFromString(state)
    | _ => None
    };
  make(state);
};

let fromStringWithDefaults = (r: string): query => {
  let q = fromString(r);
  {state: q.state |> E.O.default(`OPEN) |> E.O.some};
};

let toKeyValuePairs = (query: query): KeyValuePairs.ts => {
  let state =
    query.state
    |> E.O.fmap(statetoString)
    |> E.O.fmap(r => KeyValuePairs.make("state", r));
  state |> E.O.fmap(e => [|e|]) |> E.O.default([||]);
};

let toUrlParams = (r: query): string =>
  r |> toKeyValuePairs |> KeyValuePairs.toUrlParams;
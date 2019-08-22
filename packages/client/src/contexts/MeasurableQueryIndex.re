type state = [ | `OPEN | `JUDGEMENT_PENDING | `JUDGED | `CLOSED_AS_UNRESOLVED];

type query = {state: option(array(state))};

let make = (state: option(array(state))): query => {state: state};

let stateFromString = (r: string): array(state) =>
  switch (r) {
  | "open" => [|`OPEN|]
  | "closed" => [|`JUDGED, `CLOSED_AS_UNRESOLVED|]
  | "pending" => [|`JUDGEMENT_PENDING|]
  | _ => [|`OPEN|]
  };

let statetoString = (s: array(state)): string =>
  switch (s) {
  | [|`OPEN|] => "open"
  | [|`JUDGED, `CLOSED_AS_UNRESOLVED|] => "closed"
  | [|`JUDGEMENT_PENDING|] => "pending"
  | _ => "open"
  };

let fromString = (r: string): query => {
  let keyValuePairs = KeyValuePairs.fromSearchParam(r);
  let state =
    switch (keyValuePairs |> KeyValuePairs.get("state")) {
    | Some(state) => stateFromString(state)
    | _ => [|`OPEN|]
    };
  make(Some(state));
};

let fromStringWithDefaults = (r: string): query => {
  let q = fromString(r);
  {state: q.state};
};

let toKeyValuePairs = (query: query): KeyValuePairs.ts => {
  query.state
  |> E.O.fmap(statetoString)
  |> E.O.fmap(r => KeyValuePairs.make("state", r))
  |> E.O.fmap(e => [|e|])
  |> E.O.default([||]);
};

let toUrlParams = (r: query): string =>
  r |> toKeyValuePairs |> KeyValuePairs.toUrlParams;
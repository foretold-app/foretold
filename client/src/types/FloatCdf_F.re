let firstAbove = (min: float, t: Value.FloatCdf.t) =>
  Belt.Map.findFirstBy(t, (k, _v) => k > min);

let firstAboveValue = (min: float, t: Value.FloatCdf.t) =>
  Rationale.Option.fmap(((_, x)) => x, firstAbove(min, t));
type percentage = float;
type pointType =
  | Percentage
  | Float
  | Datetime;
type pointValue =
  | Percentage(float)
  | Float(float)
  | Datetime(string);

type trio('a) = {
  p25: 'a,
  p50: 'a,
  p75: 'a,
};

let trioIsValid = (t: trio('a)) =>
  switch (t) {
  | {p25: Percentage(a), p50: Percentage(b), p75: Percentage(c)} =>
    a <= b && b <= c
  | {p25: Float(a), p50: Float(b), p75: Float(c)} => a <= b && b <= c
  | {p25: Datetime(_), p50: Datetime(_), p75: Datetime(_)} => true
  | _ => false
  };

type point('a) = 'a;

type distribution =
  | Trio(trio(pointValue))
  | Point(pointValue);

type value = {
  pointType,
  distribution,
};
let valueIsValid = (d: distribution) =>
  switch (d) {
  | Point(_) => true
  | Trio(t) => trioIsValid(t)
  };

let encodeTrio = (t: trio('a)) =>
  switch (t.p25, t.p50, t.p75) {
  | (Percentage(a), Percentage(b), Percentage(c)) =>
    Json.Encode.(
      object_([
        ("p25", Json.Encode.float(a)),
        ("p50", Json.Encode.float(b)),
        ("p75", Json.Encode.float(c)),
      ])
    )
  | (Float(a), Float(b), Float(c)) =>
    Json.Encode.(
      object_([
        ("p25", Json.Encode.float(a)),
        ("p50", Json.Encode.float(b)),
        ("p75", Json.Encode.float(c)),
      ])
    )
  | (Datetime(a), Datetime(b), Datetime(c)) =>
    Json.Encode.(
      object_([
        ("p25", Json.Encode.string(a)),
        ("p50", Json.Encode.string(b)),
        ("p75", Json.Encode.string(c)),
      ])
    )
  | _ => raise(Not_found)
  };
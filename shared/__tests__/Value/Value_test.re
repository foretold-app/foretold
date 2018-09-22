open Jest;

open Expect;
open Value;

let () = {
  describe("FloatPercentiles", () => {
    test("#hasQuartiles true", () =>
      FloatPercentiles.fromArray([|
        (25.0, 1.0),
        (50.0, 3.0),
        (75.0, 10.0),
      |])
      |> FloatPercentiles.hasQuartiles
      |> expect
      |> toEqual(true)
    );
    test("#hasQuartiles false", () =>
      FloatPercentiles.fromArray([|
        (25.0, 1.0),
        (50.0, 3.0),
        (78.0, 10.0),
      |])
      |> FloatPercentiles.hasQuartiles
      |> expect
      |> toEqual(false)
    );
    test("#encode", () => {
      let json =
        Json.parseOrRaise(
          {| {
        "dataType": "floatByPercentile",
        "data":   { "25.": 1.0, "50.": 3.0, "78.":10.0 }
      } |},
        );
      FloatPercentiles.fromArray([|
        (25.0, 1.0),
        (50.0, 3.0),
        (78.0, 10.0),
      |])
      |> FloatPercentiles.encode
      |> expect
      |> toEqual(json);
    });
  });

  describe("Value", () => {
    test("#encode DateTimePercentiles", () => {
      let json =
        Json.parseOrRaise(
          {| {
        "dataType": "floatByPercentile",
        "data":   { "25.": 1.0, "50.": 3.0, "75.":10.0 }
      } |},
        );
      let floatPercentiles =
        FloatPercentiles.fromArray([|
          (25.0, 1.0),
          (50.0, 3.0),
          (75.0, 10.0),
        |]);
      let value = FloatPercentiles(floatPercentiles);
      value |> encodee |> expect |> toEqual(json);
    });
    test("#decode DateTimePercentiles", () => {
      let json =
        Json.parseOrRaise(
          {| {
        "dataType": "floatByPercentile",
        "data":   { "25.": 1.0, "50.": 3.0, "75.":10.0 }
      } |},
        );
      let floatPercentiles =
        FloatPercentiles.fromArray([|
          (25.0, 1.0),
          (50.0, 3.0),
          (75.0, 10.0),
        |]);
      let value = FloatPercentiles(floatPercentiles);
      json |> decode |> expect |> toEqual(value);
    });
  });
};
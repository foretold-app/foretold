open Jest;

open Belt.Result;
open Expect;
open Value;

let () = {
  describe("FloatPercentiles", () => {
    test("#decode FloatPoint", () =>
      Json.parseOrRaise(
        {| {
        "dataType": "floatPoint",
        "data": 0.3
      } |},
      )
      |> FloatPoint.decode
      |> expect
      |> toEqual(Ok(0.3))
    );

    test("#decode FloatPoint", () => {
      let value = `FloatPoint(0.3);
      Json.parseOrRaise(
        {| {
        "dataType": "floatPoint",
        "data": 0.3
      } |},
      )
      |> decode
      |> expect
      |> toEqual(Ok(value));
    });

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

  describe("Value", () =>
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
      let value = `FloatPercentiles(floatPercentiles);
      value |> encode |> expect |> toEqual(json);
    })
  );

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
    let value = `FloatPercentiles(floatPercentiles);
    json |> decode |> expect |> toEqual(Ok(value));
  });
  test("#decode with Error", () => {
    let json =
      Json.parseOrRaise(
        {| {
         "dataType": "floatByPercentile",
         "data":   { "25.": "sdf", "50.": 3.0, "75.":10.0 }
       } |},
      );
    json |> decode |> isError |> expect |> toEqual(true);
  });
};
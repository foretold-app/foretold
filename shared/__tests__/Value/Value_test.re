open Jest;

open Expect;
open Mapp;

let () =
  describe("Value", () => {
    test("DateTimePoint", () => {
      let orgVal = "sdf";
      let fromJson = orgVal |> DateTimePoint.encode |> DateTimePoint.decode;
      expect(orgVal) |> toEqual(fromJson);
    });
    test("Percentage", () => {
      let orgVal = 3.0;
      let fromJson = orgVal |> Percentage.encode |> Percentage.decode;
      expect(orgVal) |> toEqual(fromJson);
    });
    test("FloatPercentiles", () => {
      let orgVal = 3.0;
      let foo = FloatPercentiles.ByPercentile.empty;
      let bar = FloatPercentiles.ByPercentile.add(0.1, 0.2, foo);
      let fromJson = bar |> FloatPercentiles.encode;
      expect(1) |> toEqual(1);
    });
  });
/* test("next", () => {
     let toJson = Value.encodeTrio({p25: 1.0, p50: 2.0, p75: 4.1});
     let fromJson = Value.parseTrio(toJson);
     expect(fromJson.p75) |> toEqual(4.1);
   }); */
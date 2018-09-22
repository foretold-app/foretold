open Jest;

open Expect;
open Mapp;

module MakeByPercentilee = (()) => {
  module Id =
    Belt.Id.MakeComparable({
      type t = float;
      let cmp: (float, float) => int = Pervasives.compare;
    });

  type t = Belt.Map.t(Id.t, float, Id.identity);

  let hasQuertiles = (t: t) : bool =>
    Belt.Map.has(t, 25.0) && Belt.Map.has(t, 50.0) && Belt.Map.has(t, 75.0);
  let toDict = (t: t) =>
    t
    |> Belt.Map.toArray
    |> Array.map(((a, b)) => (string_of_float(a), b))
    |> Js.Dict.fromArray;

  let fromDict = (r: Js.Dict.t(float)) => {
    let foo =
      r
      |> Js.Dict.entries
      |> Array.map(((a, b)) => (float_of_string(a), b))
      |> Belt.Map.fromArray(~id=(module Id));
    foo;
  };
};

module FltCmp =
  Belt.Id.MakeComparable({
    type t = float;
    let cmp: (float, float) => int = Pervasives.compare;
  });

let aa = Belt.Map.fromArray([|(1.0, 2.0)|]);
let myMap = Belt.Map.make(~id=(module FltCmp));
/* let bb = Belt.Map.fromArray([|(1.0, 2.0)|], ~id=(module FltCmp)); */
let s0 = Belt.Map.fromArray([|(2.3, 4.0)|], ~id=(module FltCmp));

let bar =
  Belt.Map.toArray(s0) |> Array.map(((a, b)) => (string_of_float(a), b));
let char = Js.Dict.fromArray(bar);
let dictToMap =
    (a: Js.Dict.t(int))
    : Belt.Map.t(FltCmp.t, int, FltCmp.identity) => s0;

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
      expect(1) |> toEqual(1);
    });
  });
/* test("next", () => {
     let toJson = Value.encodeTrio({p25: 1.0, p50: 2.0, p75: 4.1});
     let fromJson = Value.parseTrio(toJson);
     expect(fromJson.p75) |> toEqual(4.1);
   }); */
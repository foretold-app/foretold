open Jest;

open Expect;

let () =
  describe("Value", () => {
    test("encoding / decoding", () => {
      let toJson = Value.encodeTrio({p25: 1.0, p50: 2.0, p75: 4.1});
      let fromJson = Value.parseTrio(toJson);
      expect(fromJson.p75) |> toEqual(4.1);
    });
    test("next", () => {
      let toJson = Value.encodeTrio({p25: 1.0, p50: 2.0, p75: 4.1});
      let fromJson = Value.parseTrio(toJson);
      expect(fromJson.p75) |> toEqual(4.1);
    });
  });
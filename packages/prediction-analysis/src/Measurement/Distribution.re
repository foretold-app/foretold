type t = Types.distribution;
type ts = Types.distributions;

module T = {
  module Lib = CdfLibraryImporter.Distribution;
  let hasLength = (t: t) => t.xs |> Belt.Array.length > 0;
  let empty: t = {xs: [||], ys: [||]};
  let findX = Lib.findX;
  let findY = Lib.findY;
  let make = (xs, ys): t => {xs, ys};
  let toPdf = Lib.toPdf;
  let integral = Lib.integral;
};

module Ts = {
  let minX = (x, dists) =>
    dists |> Array.map(T.findX(x)) |> E.FloatArray.min;
  let maxX = (x, dists) =>
    dists |> Array.map(T.findX(x)) |> E.FloatArray.max;
};
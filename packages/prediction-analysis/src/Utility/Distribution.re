type t = Types.distribution;
type ts = Types.distributions;

module A = {
  let fold_left = Array.fold_left;
  let fold_right = Array.fold_right;
};

module FloatArray = {
  let min = r => r |> A.fold_left((a, b) => a < b ? a : b, max_float);
  let max = r => r |> A.fold_left((a, b) => a > b ? a : b, min_float);
};

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
  let minX = (x, dists) => dists |> Array.map(T.findX(x)) |> FloatArray.min;

  let maxX = (x, dists) => dists |> Array.map(T.findX(x)) |> FloatArray.max;
};
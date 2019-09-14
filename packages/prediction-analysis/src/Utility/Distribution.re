module A = {
  let fold_left = Array.fold_left;
  let fold_right = Array.fold_right;
};

module FloatArray = {
  let min = r => r |> A.fold_left((a, b) => a < b ? a : b, max_float);
  let max = r => r |> A.fold_left((a, b) => a > b ? a : b, min_float);
};

type t = {
  xs: array(float),
  ys: array(float),
};
type ts = array(t);

module JS = {
  [@bs.deriving abstract]
  type distJs = {
    xs: array(float),
    ys: array(float),
  };

  let distToJs = (d: t) => distJs(~xs=d.xs, ~ys=d.ys);
  let jsToDist = (d: distJs) => {xs: xsGet(d), ys: ysGet(d)};

  let doAsDist = (f, d: t) => d |> distToJs |> f |> jsToDist;
  [@bs.module "./stats.js"] external cdfToPdf: distJs => distJs = "cdfToPdf";
  [@bs.module "./stats.js"] external findY: (float, distJs) => float = "findY";
  [@bs.module "./stats.js"] external findX: (float, distJs) => float = "findX";
  [@bs.module "./stats.js"] external integral: distJs => float = "integral";

  [@bs.module "./stats.js"]
  external differentialEntropy: distJs => distJs = "differentialEntropy";

  [@bs.module "./stats.js"]
  external scoreMarketCdfCdf: (distJs, distJs, distJs) => distJs =
    "scoreMarketCdfCdf";

  [@bs.module "./stats.js"]
  external scoreNonMarketCdfCdf: (distJs, distJs) => distJs =
    "scoreNonMarketCdfCdf";
};

module T = {
  let hasLength = (t: t): bool => t.xs |> Belt.Array.length > 0;
  let empty: t = {xs: [||], ys: [||]};
  let make = (xs, ys) => {xs, ys};
  let toPdf = (dist: t) => dist |> JS.doAsDist(JS.cdfToPdf);
  let requireLength = (dist: t) => dist |> hasLength ? Some(dist) : None;
  let findX = (y: float, dist: t) => dist |> JS.distToJs |> JS.findX(y);
  let findY = (x: float, dist: t) => dist |> JS.distToJs |> JS.findY(x);

  let integral = (dist: t) => dist |> JS.distToJs |> JS.integral;
  let differentialEntropy = (dist: t) =>
    dist |> JS.doAsDist(JS.differentialEntropy) |> integral;
};

module Ts = {
  let minX = (x: float, dists: ts) =>
    dists |> Array.map(T.findX(x)) |> FloatArray.min;

  let maxX = (x: float, dists: ts) =>
    dists |> Array.map(T.findX(x)) |> FloatArray.max;

  let logScoreMarketCdfCdf = (prediction: t, aggregate: t, resolution: t) => {
    JS.scoreMarketCdfCdf(
      JS.distToJs(prediction),
      JS.distToJs(aggregate),
      JS.distToJs(resolution),
    )
    |> JS.jsToDist
    |> T.integral;
  };

  let logScoreNonMarketCdfCdf = (prediction: t, resolution: t) => {
    JS.scoreNonMarketCdfCdf(
      JS.distToJs(prediction),
      JS.distToJs(resolution),
    )
    |> JS.jsToDist
    |> T.integral;
  };
};
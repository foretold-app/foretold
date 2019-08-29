module Dist = {
  type t = {
    xs: array(float),
    ys: array(float),
  };

  type asJson = {
    .
    "xs": array(float),
    "ys": array(float),
  };

  let toJson = (t: t): asJson => {"xs": t.xs, "ys": t.ys};
  let fromJson = (json: asJson): t => {xs: json##xs, ys: json##ys};

  module JS = {
    [@bs.deriving abstract]
    type distJs = {
      xs: array(float),
      ys: array(float),
    };

    let fromDist = (d: t) => distJs(~xs=d.xs, ~ys=d.ys);
    let toDist = (d: distJs) => {xs: xsGet(d), ys: ysGet(d)};

    let doAsDist = (f, d: t) => d |> fromDist |> f |> toDist;
    [@bs.module "./stats.js"] external cdfToPdf: distJs => distJs = "cdfToPdf";
    [@bs.module "./stats.js"]
    external findY: (float, distJs) => float = "findY";
    [@bs.module "./stats.js"]
    external findX: (float, distJs) => float = "findX";

    [@bs.module "./stats.js"]
    external mean: (distJs, distJs) => distJs = "mean";
  };

  let toPdf = (dist: t) => dist |> JS.doAsDist(JS.cdfToPdf);
  let mean = (dist1: t, dist2: t) => JS.mean(JS.fromDist(dist1), JS.fromDist(dist2)) |> JS.toDist;
  let findX = (y: float, dist: t) => dist |> JS.fromDist |> JS.findX(y);
  let findY = (x: float, dist: t) => dist |> JS.fromDist |> JS.findY(x);
};
type dist = {
  xs: array(float),
  ys: array(float),
};

module JS = {
  [@bs.deriving abstract]
  type distJs = {
    xs: array(float),
    ys: array(float),
  };

  let fromDist = (d: dist) => distJs(~xs=d.xs, ~ys=d.ys);
  let toDist = (d: distJs) => {xs: xsGet(d), ys: ysGet(d)};

  let doAsDist = (f, d: dist) => d |> fromDist |> f |> toDist;
  [@bs.module "./stats.js"] external cdfToPdf: distJs => distJs = "cdfToPdf";
  [@bs.module "./stats.js"] external findY: (float, distJs) => float = "findY";
  [@bs.module "./stats.js"] external findX: (float, distJs) => float = "findX";
};

let toPdf = (dist: dist) => dist |> JS.doAsDist(JS.cdfToPdf);
let findX = (y: float, dist: dist) => dist |> JS.fromDist |> JS.findX(y);
let findY = (x: float, dist: dist) => dist |> JS.fromDist |> JS.findY(x);
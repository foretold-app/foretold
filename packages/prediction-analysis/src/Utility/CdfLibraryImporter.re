// type t = Types.distribution;
type ts = Types.distributions;

module JS = {
  [@bs.deriving abstract]
  type distJs = {
    xs: array(float),
    ys: array(float),
  };

  let distToJs = (d: Types.distribution) => distJs(~xs=d.xs, ~ys=d.ys);
  let jsToDist = (d: distJs): Types.distribution => {
    xs: xsGet(d),
    ys: ysGet(d),
  };

  let doAsDist = (f, d: Types.distribution) => d |> distToJs |> f |> jsToDist;
  [@bs.module "./stats.js"] external cdfToPdf: distJs => distJs = "cdfToPdf";
  [@bs.module "./stats.js"] external findY: (float, distJs) => float = "findY";
  [@bs.module "./stats.js"] external findX: (float, distJs) => float = "findX";
  [@bs.module "./stats.js"] external integral: distJs => float = "integral";

  [@bs.module "./stats.js"]
  external differentialEntropy: (int, distJs) => distJs =
    "differentialEntropy";

  [@bs.module "./stats.js"]
  external scoreMarketCdfCdf: (int, distJs, distJs, distJs) => distJs =
    "scoreMarketCdfCdf";

  [@bs.module "./stats.js"]
  external scoreNonMarketCdfCdf: (int, distJs, distJs) => distJs =
    "scoreNonMarketCdfCdf";
};

module Distribution = {
  let toPdf = (dist: Types.distribution) => dist |> JS.doAsDist(JS.cdfToPdf);
  let findX = (y: float, dist: Types.distribution) =>
    dist |> JS.distToJs |> JS.findX(y);
  let findY = (x: float, dist: Types.distribution) =>
    dist |> JS.distToJs |> JS.findY(x);
  let integral = (dist: Types.distribution) =>
    dist |> JS.distToJs |> JS.integral;
  let differentialEntropy = (maxCalculationLength, dist: Types.distribution) =>
    dist
    |> JS.doAsDist(JS.differentialEntropy(maxCalculationLength))
    |> integral;
};

module PredictionResolutionGroup = {
  let logScoreMarketCdfCdf =
      (
        ~sampleCount,
        ~agentPrediction: Types.distribution,
        ~marketPrediction: Types.distribution,
        ~resolution: Types.distribution,
      ) => {
    JS.scoreMarketCdfCdf(
      sampleCount,
      JS.distToJs(agentPrediction),
      JS.distToJs(marketPrediction),
      JS.distToJs(resolution),
    )
    |> JS.jsToDist
    |> Distribution.integral;
  };

  let logScoreNonMarketCdfCdf =
      (
        ~sampleCount,
        ~agentPrediction: Types.distribution,
        ~resolution: Types.distribution,
      ) => {
    JS.scoreNonMarketCdfCdf(
      sampleCount,
      JS.distToJs(agentPrediction),
      JS.distToJs(resolution),
    )
    |> JS.jsToDist
    |> Distribution.integral;
  };
};
open Rationale.Function;

let findX = (x: float) => Cdf.toDistribution ||> Distribution.T.findX(x);
let findY = (y: float) => Cdf.toDistribution ||> Distribution.T.findY(y);
let integral = Cdf.toDistribution ||> Distribution.T.integral;

let differentialEntropy = (t: Cdf.t) =>
  LogError.differentialEntropy(10000, `Cdf(t));
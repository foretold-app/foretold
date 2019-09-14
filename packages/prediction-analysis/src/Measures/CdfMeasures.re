type t = Cdf.t;
open Rationale.Function;

let findX = (x: float) => Cdf.toDistribution ||> Distribution.T.findX(x);
let findY = (y: float) => Cdf.toDistribution ||> Distribution.T.findY(y);
let integral = Cdf.toDistribution ||> Distribution.T.integral;
let toPdf = Cdf.toDistribution ||> Distribution.T.toPdf;
let differentialEntropy =
  Cdf.toDistribution ||> Distribution.T.differentialEntropy;
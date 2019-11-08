open Jest;
open Expect;

let mkPer = Percentage.makeWithoutValidations;
let mkC = Cdf.makeWithoutValidations;

describe("#scorePointCombination", () => {
  test("with PercentagePercentage score", () => {
    let scoringCombination: PredictionResolutionGroup.t =
      `PercentagePercentage({
        agentPrediction: mkPer(0.01),
        marketPrediction: Some(mkPer(0.4)),
        resolution: mkPer(0.01),
      });
    expect(
      PredictionResolutionGroupMeasures.pointScore(
        ~marketType=`MarketScore,
        ~scoringCombination,
        (),
      )
      |> Belt.Result.getExn,
    )
    |> toBeCloseTo(0.662);
  });
  test("with CdfCdf score", () => {
    let scoringCombination: PredictionResolutionGroup.t =
      `CdfCdf({
        agentPrediction:
          mkC(~xs=[|1., 2., 3., 4.|], ~ys=[|0., 0.3, 0.6, 1.|]),
        marketPrediction:
          Some(mkC(~xs=[|1., 2., 3., 4.|], ~ys=[|0., 0.1, 0.1, 1.|])),
        resolution: mkC(~xs=[|1., 2., 3., 4.|], ~ys=[|0., 0.8, 0.9, 1.|]),
      });
    expect(
      PredictionResolutionGroupMeasures.pointScore(
        ~marketType=`MarketScore,
        ~scoringCombination,
        (),
      )
      |> Belt.Result.getExn,
    )
    |> toBeCloseTo(16.85596797789106);
  });
});

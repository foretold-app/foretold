open Jest;
open Expect;

let mkPer = Percentage.makeWithoutValidations;

describe("#scorePointCombination", () =>
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
  })
);
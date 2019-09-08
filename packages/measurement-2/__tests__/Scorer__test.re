open Jest;
open Expect;

let maker = {
  open TypedMeasurementWithTime.TypedMeasurementWithTime;
  let combinationOverTime: ScoringCombinationOverTime.MeasurementCombinationOverTime.t =
    `PercentagePercentage({
      agentPredictions: [|make(0., 0.99), make(1., 0.11)|],
      marketPredictions: Some([|make(0., 0.5), make(5., 0.7)|]),
      resolution: make(10., 0.99),
    });
  combinationOverTime;
};

describe("Scorer", () => {
  describe("#scorePointCombination", () =>
    test("with PercentagePercentage score", () => {
      let scoringCombination: ScoringCombination.ValidScoringCombination.t =
        `PercentagePercentage({
          agentPrediction: 0.01,
          marketPrediction: Some(0.4),
          resolution: 0.01,
        });
      expect(
        Scorer.scorePointCombination(
          ~marketType=MarketScore,
          ~scoringCombination,
          (),
        )
        |> Belt.Result.getExn,
      )
      |> toBeCloseTo(0.662);
    })
  );
  describe("#scorePointCombination", () =>
    test("with PercentagePercentage score", () =>
      expect(
        Scorer.scoredIntegralOverTime(
          ~marketType=MarketScore,
          ~scoringCombination={beginningTime: 0.0, measurementGroup: maker},
          (),
        )
        |> Belt.Result.getExn,
      )
      |> toBeCloseTo(0.662)
    )
  );
});
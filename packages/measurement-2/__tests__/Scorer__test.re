open Jest;
open Expect;

describe("Scorer", () => {
  describe("#scorePointCombination", () =>
    test("with PercentagePercentage score", () => {
      open Percentage;
      let scoringCombination: PredictionResolutionGroup.t =
        `PercentagePercentage({
          agentPrediction: makeExt(0.01),
          marketPrediction: Some(makeExt(0.4)),
          resolution: makeExt(0.01),
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
    test("with PercentagePercentage score", () => {
      let makeCombination = {
        open TypedMeasurementWithTime.TypedMeasurementWithTime;
        let makeExt = Percentage.makeExt;
        let combinationOverTime: ScoringCombinationOverTime.MeasurementCombinationOverTime.t =
          `PercentagePercentage({
            agentPredictions: [|
              make(3., makeExt(0.70)),
              make(8., makeExt(0.88)),
            |],
            marketPredictions:
              Some([|make(0., makeExt(0.5)), make(5., makeExt(0.7))|]),
            resolution: make(10., makeExt(0.95)),
          });
        combinationOverTime;
      };

      expect(
        Scorer.scoredIntegralOverTime(
          ~marketType=MarketScore,
          ~scoringCombination={
            beginningTime: 0.0,
            measurementGroup: makeCombination,
          },
          (),
        )
        |> Belt.Result.getExn,
      )
      |> toBeCloseTo(1.34);
    })
  );
});
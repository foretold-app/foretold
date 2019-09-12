open Jest;
open Expect;

let mkPer = Percentage.makeWithoutValidations;

describe("Scorer", () => {
  describe("#scorePointCombination", () =>
    test("with PercentagePercentage score", () => {
      let scoringCombination: PredictionResolutionGroup.t =
        `PercentagePercentage({
          agentPrediction: mkPer(0.01),
          marketPrediction: Some(mkPer(0.4)),
          resolution: mkPer(0.01),
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
        open PredictionResolutionOverTime.MeasurementWithTime;
        let combinationOverTime: PredictionResolutionOverTime.T.t =
          `PercentagePercentage({
            agentPredictions: [|
              make(~time=3., ~measurementValue=mkPer(0.70), ()),
              make(~time=8., ~measurementValue=mkPer(0.88), ()),
            |],
            marketPredictions:
              Some([|
                make(~time=0., ~measurementValue=mkPer(0.5), ()),
                make(~time=5., ~measurementValue=mkPer(0.7), ()),
              |]),
            resolution: make(~time=10., ~measurementValue=mkPer(0.95), ()),
          });
        combinationOverTime;
      };

      expect(
        Scorer.scoredIntegralOverTime(
          ~marketType=MarketScore,
          ~scoringCombination=makeCombination,
          (),
        )
        |> Belt.Result.getExn,
      )
      |> toBeCloseTo(1.34);
    })
  );
});
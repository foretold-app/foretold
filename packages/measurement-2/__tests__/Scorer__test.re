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
        open PredictionResolutionOverTime.MeasurementWithTime;
        let makeExt = Percentage.makeExt;
        let combinationOverTime: PredictionResolutionOverTime.T.t =
          `PercentagePercentage({
            agentPredictions: [|
              make(~time=3., ~measurementValue=makeExt(0.70), ()),
              make(~time=8., ~measurementValue=makeExt(0.88), ()),
            |],
            marketPredictions:
              Some([|
                make(~time=0., ~measurementValue=makeExt(0.5), ()),
                make(~time=5., ~measurementValue=makeExt(0.7), ()),
              |]),
            resolution: make(~time=10., ~measurementValue=makeExt(0.95), ()),
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
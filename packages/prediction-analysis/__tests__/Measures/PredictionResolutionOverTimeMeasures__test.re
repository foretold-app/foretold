open Jest;
open Expect;

let mkPer = Percentage.makeWithoutValidations;

describe("#scorePointCombination", () =>
  test("with PercentagePercentage score", () => {
    let makeCombination = {
      open PredictionResolutionOverTime.MeasurementWithTime;
      let combinationOverTime: PredictionResolutionOverTime.t =
        `PercentagePercentage({
          agentPredictions: [|
            make(~time=3., ~measurementValue=mkPer(0.70)),
            make(~time=8., ~measurementValue=mkPer(0.88)),
          |],
          marketPredictions:
            Some([|
              make(~time=0., ~measurementValue=mkPer(0.5)),
              make(~time=5., ~measurementValue=mkPer(0.7)),
            |]),
          resolution: make(~time=10., ~measurementValue=mkPer(0.95)),
        });
      combinationOverTime;
    };

    expect(
      PredictionResolutionOverTimeMeasures.averagePointScore(
        ~marketType=`MarketScore,
        ~scoringCombination=makeCombination,
        (),
      )
      |> Belt.Result.getExn,
    )
    |> toBeCloseTo(0.268);
  })
);
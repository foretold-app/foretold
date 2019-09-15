var {MeasurementValue, PredictionResolutionGroup, marketScore, nonMarketScore, PredictionResolutionOverTime} = require("../../src/JsExport/JsLib");

describe("#MeasurementValue", () => {
  test('Constructor', () => {
      expect((new MeasurementValue({data: 0.2, dataType: "percentage"})).error).toBe(undefined);
    });

  test('Has error with bad dataType', () => {
      expect((new MeasurementValue({data: 0.2, dataType: "xyz"})).error).toBe("Measurement datatype value not valid: xyz");
    });

  test('#differentialEntropy', () => {
    let m = new MeasurementValue({data: 0.5, dataType: "percentage"});
      expect(m.differentialEntropy().data).toBe(-1);
    });

  test('#differentialEntropy', () => {
    let m = new MeasurementValue({data: {xs: [1,2,3,4,5], ys: [.1,.2,.3, .4, 1.]}, dataType: "floatCdf"});
      expect(m.differentialEntropy().data).toBeCloseTo(-1.481);
    });
});

describe("#PredictionResolutionGroup", () => {
  test('Constructor', () => {
      expect((new PredictionResolutionGroup({
        agentPrediction: {data: 0.2, dataType: "percentage"},
        marketPrediction: {data: 0.2, dataType: "percentage"},
        resolution:{data: 0.2, dataType: "percentage"}
      })).error).toBe(undefined);
    });

  test('#PointScore', () => {
      expect((new PredictionResolutionGroup({
        agentPrediction: {data: 0.1, dataType: "percentage"},
        marketPrediction: {data: 0.6, dataType: "percentage"},
        resolution:{data: 0.05, dataType: "percentage"}
      })).pointScore(marketScore).data).toBeCloseTo(0.982);
    });
})

describe("#PredictionResolutionOverTime", () => {
  test('Constructor', () => {
      expect((new PredictionResolutionOverTime({
        agentPredictions: [
          {time: 10., measurement: {data: 0.4, dataType: "percentage"}},
          {time: 20., measurement: {data: 0.3, dataType: "percentage"}},
          {time: 30., measurement: {data: 0.2, dataType: "percentage"}}
        ],
        marketPredictions: [
          {time: 5., measurement: {data: 0.8, dataType: "percentage"}},
          {time: 15., measurement: {data: 0.7, dataType: "percentage"}},
          {time: 25., measurement: {data: 0.6, dataType: "percentage"}}
        ],
        resolution: {time: 40, measurement: {data: 0.2, dataType: "percentage"}}
      })).error).toBe(undefined);
    });

  test('#averagePointScore', () => {
      expect((new PredictionResolutionOverTime({
        agentPredictions: [
          {time: 3., measurement: {data: 0.7, dataType: "percentage"}},
          {time: 8., measurement: {data: 0.88, dataType: "percentage"}},
        ],
        marketPredictions: [
          {time: 0., measurement: {data: 0.5, dataType: "percentage"}},
          {time: 5., measurement: {data: 0.7, dataType: "percentage"}},
        ],
        resolution: {time: 10, measurement: {data: 0.95, dataType: "percentage"}}
      })).averagePointScore(marketScore, undefined).data).toBeCloseTo(0.268);
    });

  test('#averagePointScore with low point', () => {
      expect((new PredictionResolutionOverTime({
        agentPredictions: [
          {time: 3., measurement: {data: 0.7, dataType: "percentage"}},
          {time: 8., measurement: {data: 0.88, dataType: "percentage"}},
        ],
        marketPredictions: [
          {time: 0., measurement: {data: 0.5, dataType: "percentage"}},
          {time: 5., measurement: {data: 0.7, dataType: "percentage"}},
        ],
        resolution: {time: 10, measurement: {data: 0.95, dataType: "percentage"}}
      })).averagePointScore(marketScore, 1.0).data).toBeCloseTo(0.192);
    });
});
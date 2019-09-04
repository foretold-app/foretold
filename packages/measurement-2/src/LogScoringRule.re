open Belt.Result;

let logError = Js.Math.log2;
open Types.ValidScoringCombination;

let marketCdfCdf =
    (
      {prediction, market, resolution, sampleCount}: Types.ValidScoringCombination.marketCdfCdf,
    ) =>
  Ok(3.0);

let marketCdfFloat =
    (
      {prediction, market, resolution}: Types.ValidScoringCombination.marketCdfFloat,
    ) =>
  Ok(3.0);

let marketPercentagePercentage =
    (
      {prediction, market, resolution}: Types.ValidScoringCombination.marketPercentagePercentage,
    ) =>
  Ok(
    //   TODO: Handle cases where market is 0 or 1
    Types.Percentage.(
      {
        let positive = resolution *. logError(prediction /. market);
        let negative =
          inverse(resolution)
          *. logError(inverse(prediction) /. inverse(market));
        positive +. negative;
      }
    ),
  );

let nonMarketCdfCdf =
    (
      {prediction, resolution, sampleCount}: Types.ValidScoringCombination.nonMarketCdfCdf,
    ) =>
  Ok(3.0);

let nonMarketCdfFloat =
    (
      {prediction, resolution}: Types.ValidScoringCombination.nonMarketCdfFloat,
    ) =>
  Ok(3.0);

let nonMarketPercentagePercentage =
    (
      {prediction, resolution}: Types.ValidScoringCombination.nonMarketPercentagePercentage,
    ) =>
  Ok(
    Types.Percentage.(
      {
        let positive = resolution *. logError(prediction /. resolution);
        let negative =
          inverse(resolution)
          *. logError(inverse(prediction) /. inverse(resolution));
        positive +. negative;
      }
    ),
  );
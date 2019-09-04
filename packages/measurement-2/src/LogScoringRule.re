open Belt.Result;

let logError = Js.Math.log2;

module MarketScores = {
  let cdfCdfCdf =
      (prediction: Types.Cdf.t, market: Types.Cdf.t, resolution: Types.Cdf.t) =>
    Ok(3.0);

  let cdfCdfFloat =
      (prediction: Types.Cdf.t, market: Types.Cdf.t, resolution: float) =>
    Ok(3.0);

  let percentagePercentagePercentage =
      (
        prediction: Types.Percentage.t,
        market: Types.Percentage.t,
        resolution: Types.Percentage.t,
      ) =>
    Ok(
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
};

module NonMarketScores = {
  let cdfCdf = (prediction: Types.Cdf.t, resolution: Types.Cdf.t) => Ok(3.0);

  let cdfFloat = (prediction: Types.Cdf.t, resolution: float) => Ok(3.0);

  let percentagePercentage =
      (prediction: Types.Percentage.t, resolution: Types.Percentage.t) => {
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
  };
};

let run = ({measurements}: ScoringCombination.t) => {
  switch (
    {
      measurements;
    }
  ) {
  | `marketScoreMeasurements({prediction, market, resolution}) =>
    switch (prediction, market, resolution) {
    | (`Cdf(a), `Cdf(b), `Cdf(c)) => MarketScores.cdfCdfCdf(a, b, c)
    | (`Cdf(a), `Cdf(b), `Float(c)) => MarketScores.cdfCdfFloat(a, b, c)
    | (`Percentage(a), `Percentage(b), `Percentage(c)) =>
      MarketScores.percentagePercentagePercentage(a, b, c)
    | _ => Error("Combination not supported")
    }
  | `nonMarketScoreMeasurements({prediction, resolution}) =>
    switch (prediction, resolution) {
    | (`Cdf(a), `Cdf(b)) => NonMarketScores.cdfCdf(a, b)
    | (`Cdf(a), `Float(b)) => NonMarketScores.cdfFloat(a, b)
    | (`Percentage(a), `Percentage(b)) =>
      NonMarketScores.percentagePercentage(a, b)
    | _ => Error("Combination not supported")
    }
  };
};
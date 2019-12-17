# Prediction Analysis Library

This is a [ReasonML](https://reasonml.github.io/) library to organize 
and score predictions.

## Measurement Value

There are several kinds of values relevant for predictions used in Foretold. 
Below are the most complicated.

### Cdf

"Cdf" stand for [cumulative distribution functions](https://en.wikipedia.org/wiki/Cumulative_distribution_function).

## PredictionResolutionGroup

A "Prediction Resolution Group" is a group of measurements needed to score a 
particular prediction. This always requires a prediction and a resolution of 
similar types. Some scores also require a market prediction as well.

## PredictionResolutionOverTime

A "Prediction Resolution Over Time" is a group of measurements needed to 
score a user's predictions over a period of time. This is done by making 
a bunch of Prediction Resolution Groups, scoring those, and taking the average 
user score over a specific period.

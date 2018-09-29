type id = string;
type timestamp = string;

module Values = {
  type x =
    | Float
    | DateTime;

  type trio('a) = {
    p25: 'a,
    p50: 'a,
    p75: 'a,
  };

  type point('a) = {point: 'a};

  type distributionInPercentiles('a) = {percentiles: Js.Dict.t('a)};

  type distributionInValues('a) = {values: array('a)};

  type dataFormat =
    | Point
    | Tripple
    | DistributionInPercentiles
    | DistributionInValues;

  type dataType =
    | Float
    | Percentage
    | DateTime;

  type value = {
    dataFormat,
    dataType,
    isEstimate: bool,
  };
};

module Agents = {
  type user = {
    id,
    name: string,
  };

  type summaryBot = {
    id,
    user,
    name: string,
    description: string,
  };

  type submissionBot = {
    id,
    user,
    name: string,
    description: string,
  };

  type objectiveBot = {
    id,
    user,
    name: string,
    description: string,
  };

  type agent =
    | User(user)
    | SummaryBot(summaryBot)
    | SubmissionBot(submissionBot);

  type predictiveAgent =
    | User(user)
    | SummaryBot(summaryBot)
    | SubmissionBot(submissionBot);

  type objectiveAgent =
    | User(user)
    | SubmissionBot(submissionBot);
};

module Measurables = {
  type predictiveMeasurable = {
    name: string,
    allowedObjectiveAgents: array(Agents.objectiveAgent),
    dataType: Values.dataType,
    isLocked: bool,
    lockedAt: timestamp,
  };

  type objectiveMeasurable = {
    name: string,
    allowedObjectiveAgents: array(Agents.objectiveAgent),
  };

  type measurable =
    | PredictiveMeasurable(predictiveMeasurable)
    | ObjectiveMeasurable(objectiveMeasurable);
};

module Measurements = {
  type objectiveMeasurement = {
    measurable: Measurables.measurable,
    value: Values.value,
    agent: Agents.objectiveAgent,
    createdAt: timestamp,
    relevantAt: timestamp,
  };

  type submissionMeasurement = {
    predictiveMeasurable: Measurables.predictiveMeasurable,
    value: Values.value,
    agent: Agents.predictiveAgent,
    createdAt: timestamp,
  };

  type summaryMeasurement = {
    measurable: Measurables.measurable,
    value: Values.value,
    agent: Agents.summaryBot,
    createdAt: timestamp,
    taggedMeasurement: submissionMeasurement,
    taggedMeasurementCreatedAt: timestamp,
  };

  type measurement =
    | ObjectiveMeasurement(objectiveMeasurement)
    | SubmissionMeasurement(submissionMeasurement)
    | SummaryMeasurement(summaryMeasurement);
};

type bounty = {
  measurable: Measurables.measurable,
  agent: Agents.agent,
  value: int,
};

type metricType =
  | DateTime
  | Float
  | Percentage;

type metric = {
  typee: metricType,
  primary: Measurables.measurable,
  cancelation: Measurables.measurable,
};
const graphql = require('graphql');
const { resolver, DateType } = require('graphql-sequelize');

const models = require('../models');
const { MEASUREMENT_VALUE } = require('../models/enums/measurement-value');

const { measurementUnresolvableResolution } = require('./enums/measurement-unresolvable-resolution');
const { measurementCommentType } = require('./enums/measurement-comment-type');

const measurementValueInputFloatCdf = new graphql.GraphQLInputObjectType({
  name: 'MeasurementValueInputFloatCdf',
  fields: () => ({
    xs: { type: graphql.GraphQLNonNull(graphql.GraphQLList(graphql.GraphQLFloat)) },
    ys: { type: graphql.GraphQLNonNull(graphql.GraphQLList(graphql.GraphQLFloat)) },
  }),
});

const measurementValueFloatCdf = new graphql.GraphQLObjectType({
  name: 'MeasurementValueFloatCdf',
  fields: () => ({
    xs: { type: graphql.GraphQLNonNull(graphql.GraphQLList(graphql.GraphQLNonNull(graphql.GraphQLFloat))) },
    ys: { type: graphql.GraphQLNonNull(graphql.GraphQLList(graphql.GraphQLNonNull(graphql.GraphQLFloat))) },
  }),
});

const measurementValueInput = new graphql.GraphQLInputObjectType({
  name: 'MeasurementValueInput',
  fields: () => ({
    [MEASUREMENT_VALUE.floatCdf]: { type: measurementValueInputFloatCdf },
    [MEASUREMENT_VALUE.floatPoint]: { type: graphql.GraphQLFloat },
    [MEASUREMENT_VALUE.percentage]: { type: graphql.GraphQLFloat },
    [MEASUREMENT_VALUE.binary]: { type: graphql.GraphQLBoolean },
    [MEASUREMENT_VALUE.unresolvableResolution]: { type: measurementUnresolvableResolution },
    [MEASUREMENT_VALUE.comment]: { type: measurementCommentType },
  }),
});

const measurementValue = new graphql.GraphQLObjectType({
  name: 'MeasurementValue',
  fields: () => ({
    [MEASUREMENT_VALUE.floatCdf]: { type: measurementValueFloatCdf },
    [MEASUREMENT_VALUE.floatPoint]: { type: graphql.GraphQLFloat },
    [MEASUREMENT_VALUE.percentage]: { type: graphql.GraphQLFloat },
    [MEASUREMENT_VALUE.binary]: { type: graphql.GraphQLBoolean },
    [MEASUREMENT_VALUE.unresolvableResolution]: { type: measurementUnresolvableResolution },
    [MEASUREMENT_VALUE.comment]: { type: measurementCommentType },
  }),
});

const measurementCreateInput = new graphql.GraphQLInputObjectType({
  name: 'MeasurementCreateInput',
  fields: () => ({
    value: { type: measurementValueInput },
    valueText: { type: graphql.GraphQLString },
    competitorType: { type: require('./enums/measurement-competitor-type').measurementCompetitorType },
    measurableId: { type: graphql.GraphQLString },
    agentId: { type: graphql.GraphQLString },
    description: { type: graphql.GraphQLString },
    taggedMeasurementId: { type: graphql.GraphQLString },
    relevantAt: { type: DateType.default },
  }),
});

const measurementsInDateRangeInput = new graphql.GraphQLInputObjectType({
  name: 'MeasurementsInDateRangeInput',
  fields: () => ({
    startDate: { type: DateType.default },
    endDate: { type: DateType.default },
    spacedLimit: { type: graphql.GraphQLInt },
  }),
});

const measurement = new graphql.GraphQLObjectType({
  name: 'Measurement',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    value: { type: graphql.GraphQLNonNull(measurementValue) },
    competitorType: { type: require('./enums/measurement-competitor-type').measurementCompetitorType },
    description: { type: graphql.GraphQLString },
    measurableId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    agentId: { type: graphql.GraphQLString },
    relevantAt: { type: DateType.default },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    taggedMeasurementId: { type: graphql.GraphQLString },
    iAmOwner: require('./common').iAmOwner,
    valueText: { type: graphql.GraphQLString },
    measurementScoreSet: { type: require('./measurements').measurementScoreSet },

    Measurable: {
      type: require('./measurables').measurable,
      resolve: resolver(models.Measurement.Measurable),
    },

    Agent: {
      type: require('./agents').agent,
      resolve: resolver(models.Measurement.Agent),
    },

    TaggedMeasurement: {
      type: measurement,
      resolve: resolver(models.Measurement.TaggedMeasurement),
    },

    TaggedBy: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(measurement)),
      resolve: resolver(models.Measurement.TaggedBy),
    },
  })
});

const measurementsEdge = new graphql.GraphQLObjectType({
  name: 'MeasurementsEdge',
  fields: () => ({
    node: { type: require('./measurements').measurement },
    cursor: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const measurementsConnection = new graphql.GraphQLObjectType({
  name: 'MeasurementsConnection',
  fields: () => ({
    total: { type: graphql.GraphQLInt },
    pageInfo: { type: graphql.GraphQLNonNull(require('./common').pageInfoConnection) },
    edges: { type: graphql.GraphQLList(require('./measurements').measurementsEdge) },
  }),
});

const agentMeasurementsEdge = new graphql.GraphQLObjectType({
  name: 'AgentMeasurementsEdge',
  fields: () => ({
    node: { type: require('./measurements').measurement },
    cursor: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const agentMeasurementsConnection = new graphql.GraphQLObjectType({
  name: 'AgentMeasurementsConnection',
  fields: () => ({
    total: { type: graphql.GraphQLInt },
    pageInfo: { type: graphql.GraphQLNonNull(require('./common').pageInfoConnection) },
    edges: { type: graphql.GraphQLList(require('./measurements').agentMeasurementsEdge) },
  }),
});

const measurementScoreSet = new graphql.GraphQLObjectType({
  name: 'MeasurementScoreSet',
  fields: () => ({
    prediction: { type: graphql.GraphQLNonNull(require('./measurements').measurement) },
    outcome: { type: graphql.GraphQLNonNull(require('./measurements').measurement) },
    previousAggregate: { type: require('./measurements').measurement },
    primaryPointScore: { type: graphql.GraphQLFloat, resolve: () => 0.1 },
  }),
});

module.exports = {
  measurement,
  measurementCreateInput,

  measurementsEdge,
  measurementsConnection,
  agentMeasurementsEdge,
  agentMeasurementsConnection,

  measurementsInDateRangeInput,

  measurementScoreSet,
};

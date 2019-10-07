const graphql = require('graphql');
const { resolver, DateType } = require('graphql-sequelize');
const {
  MEASUREMENT_VALUE,
} = require('@foretold/measurement-value/enums/measurement-value');

const models = require('../../models');

const { objectId } = require('./scalars');
const { string256, string8K } = require('./scalars');

const {
  measurementUnresolvableResolution,
} = require('./enums/measurement-unresolvable-resolution');
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
    [MEASUREMENT_VALUE.floatCdf]: {
      type: measurementValueFloatCdf,
      args: {
        truncate: { type: graphql.GraphQLFloat },
        round: { type: graphql.GraphQLInt },
      },
      resolve: require('../resolvers').measurements.truncateCdf,
    },
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
    valueText: { type: string256 },
    competitorType: { type: require('./enums/measurement-competitor-type').measurementCompetitorType },
    measurableId: { type: objectId },
    agentId: { type: objectId },
    description: { type: string8K },
    taggedMeasurementId: { type: objectId },
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
    id: { type: graphql.GraphQLNonNull(objectId) },
    value: { type: graphql.GraphQLNonNull(measurementValue) },
    competitorType: { type: require('./enums/measurement-competitor-type').measurementCompetitorType },
    description: { type: string8K },
    measurableId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    agentId: { type: objectId },
    relevantAt: { type: DateType.default },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    taggedMeasurementId: { type: objectId },
    iAmOwner: require('./common').iAmOwner,
    valueText: { type: string256 },

    measurementScoreSet: {
      type: require('./measurements').measurementScoreSet,
      resolve: require('../resolvers/measurements').scoreSet,
    },

    measurable: {
      type: require('./measurables').measurable,
      resolve: resolver(models.Measurement.Measurable),
    },

    agent: {
      type: require('./agents').agent,
      resolve: resolver(models.Measurement.Agent),
    },

    taggedMeasurement: {
      type: measurement,
      resolve: resolver(models.Measurement.TaggedMeasurement),
    },

    taggedBy: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(measurement)),
      resolve: resolver(models.Measurement.TaggedBy),
    },
  }),
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
    prediction: {
      description: 'Returns itself.',
      type: graphql.GraphQLNonNull(require('./measurements').measurement),
      resolve: require('../resolvers/measurements').prediction,
    },
    outcome: {
      description: 'Returns latest objective measurement.',
      type: require('./measurements').measurement,
      resolve: require('../resolvers/measurements').outcome,
    },
    previousAggregate: {
      description: 'Returns latest aggregation measurement.',
      type: require('./measurements').measurement,
      resolve: require('../resolvers/measurements').previousAggregate,
    },
    nonMarketLogScore: {
      description: 'Not fully implemented yet.',
      type: graphql.GraphQLFloat,
      resolve: require('../resolvers/measurements').nonMarketLogScore,
    },
    primaryPointScore: {
      description: 'Not fully implemented yet.',
      type: graphql.GraphQLFloat,
      resolve: require('../resolvers/measurements').primaryPointScore,
    },
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

const graphql = require('graphql');
const { resolver, DateType } = require('graphql-sequelize');
const { MEASUREMENT_VALUE } = require('@foretold/measurement-value/enums');

const resolvers = require('../resolvers');
const models = require('../../models/definitions');

const votesTypes = require('./votes');
const measurablesTypes = require('./measurables');
const agentsTypes = require('./agents');
const permissionsTypes = require('./permissions');
const commonTypes = require('./common');

const { measurementUnresolvableResolution } = require('./enums');
const { measurementCommentType } = require('./enums');
const { measurementCompetitorType } = require('./enums');

const measurementValueInputFloatCdf = new graphql.GraphQLInputObjectType({
  name: 'MeasurementValueInputFloatCdf',
  fields: () => ({
    xs: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(graphql.GraphQLFloat)),
    },
    ys: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(graphql.GraphQLFloat)),
    },
  }),
});

const measurementValueFloatCdf = new graphql.GraphQLObjectType({
  name: 'MeasurementValueFloatCdf',
  fields: () => ({
    xs: {
      type: graphql.GraphQLNonNull(
        graphql.GraphQLList(graphql.GraphQLNonNull(graphql.GraphQLFloat)),
      ),
    },
    ys: {
      type: graphql.GraphQLNonNull(
        graphql.GraphQLList(graphql.GraphQLNonNull(graphql.GraphQLFloat)),
      ),
    },
  }),
});

const measurementValueInput = new graphql.GraphQLInputObjectType({
  name: 'MeasurementValueInput',
  fields: () => ({
    [MEASUREMENT_VALUE.floatCdf]: { type: measurementValueInputFloatCdf },
    [MEASUREMENT_VALUE.floatPoint]: { type: graphql.GraphQLFloat },
    [MEASUREMENT_VALUE.percentage]: { type: graphql.GraphQLFloat },
    [MEASUREMENT_VALUE.binary]: { type: graphql.GraphQLBoolean },
    [MEASUREMENT_VALUE.unresolvableResolution]: {
      type: measurementUnresolvableResolution,
    },
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
      resolve: resolvers.measurements.truncateCdf,
    },
    [MEASUREMENT_VALUE.floatPoint]: { type: graphql.GraphQLFloat },
    [MEASUREMENT_VALUE.percentage]: { type: graphql.GraphQLFloat },
    [MEASUREMENT_VALUE.binary]: { type: graphql.GraphQLBoolean },
    [MEASUREMENT_VALUE.unresolvableResolution]: {
      type: measurementUnresolvableResolution,
    },
    [MEASUREMENT_VALUE.comment]: { type: measurementCommentType },
  }),
});

const measurementCreateInput = new graphql.GraphQLInputObjectType({
  name: 'MeasurementCreateInput',
  fields: () => ({
    value: { type: measurementValueInput },
    valueText: { type: graphql.GraphQLString },
    competitorType: {
      type: measurementCompetitorType,
    },
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
    competitorType: {
      type: measurementCompetitorType,
    },
    description: { type: graphql.GraphQLString },
    measurableId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    agentId: { type: graphql.GraphQLString },
    relevantAt: { type: DateType.default },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    taggedMeasurementId: { type: graphql.GraphQLString },
    iAmOwner: commonTypes.iAmOwner,
    valueText: { type: graphql.GraphQLString },

    measurementScoreSet: {
      type: measurementScoreSet,
      resolve: require('../resolvers/measurements').scoreSet,
    },

    totalVoteAmount: {
      type: graphql.GraphQLInt,
      resolve: require('../resolvers/votes').total,
    },

    vote: {
      type: votesTypes.vote,
      resolve: require('../resolvers/votes').oneByMeasurementId,
    },

    // @todo: Do not use resolver. Use common interfaces of Data layer.
    measurable: {
      type: measurablesTypes.measurable,
      resolve: resolver(models.Measurement.Measurable),
    },

    // @todo: Do not use resolver. Use common interfaces of Data layer.
    agent: {
      type: agentsTypes.agent,
      resolve: resolver(models.Measurement.Agent),
    },

    // @todo: Do not use resolver. Use common interfaces of Data layer.
    taggedMeasurement: {
      type: measurement,
      resolve: resolver(models.Measurement.TaggedMeasurement),
    },

    // @todo: Do not use resolver. Use common interfaces of Data layer.
    taggedBy: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(measurement)),
      resolve: resolver(models.Measurement.TaggedBy),
    },

    permissions: {
      type: graphql.GraphQLNonNull(permissionsTypes.permissions),
      resolve: resolvers.permissions.measurementsPermissions,
    },
  }),
});

const measurementsEdge = new graphql.GraphQLObjectType({
  name: 'MeasurementsEdge',
  fields: () => ({
    node: { type: measurement },
    cursor: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const measurementsConnection = new graphql.GraphQLObjectType({
  name: 'MeasurementsConnection',
  fields: () => ({
    total: { type: graphql.GraphQLInt },
    pageInfo: {
      type: graphql.GraphQLNonNull(commonTypes.pageInfoConnection),
    },
    edges: {
      type: graphql.GraphQLList(measurementsEdge),
    },
  }),
});

const agentMeasurementsEdge = new graphql.GraphQLObjectType({
  name: 'AgentMeasurementsEdge',
  fields: () => ({
    node: { type: measurement },
    cursor: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const agentMeasurementsConnection = new graphql.GraphQLObjectType({
  name: 'AgentMeasurementsConnection',
  fields: () => ({
    total: { type: graphql.GraphQLInt },
    pageInfo: {
      type: graphql.GraphQLNonNull(commonTypes.pageInfoConnection),
    },
    edges: {
      type: graphql.GraphQLList(agentMeasurementsEdge),
    },
  }),
});

const measurementScoreSet = new graphql.GraphQLObjectType({
  name: 'MeasurementScoreSet',
  fields: () => ({
    prediction: {
      description: 'Returns itself.',
      type: graphql.GraphQLNonNull(measurement),
      resolve: require('../resolvers/measurements').prediction,
    },
    outcome: {
      description: 'Returns latest objective measurement.',
      type: measurement,
      resolve: require('../resolvers/measurements').outcome,
    },
    previousAggregate: {
      description: 'Returns latest aggregation measurement.',
      type: measurement,
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
  agentMeasurementsConnection,
  agentMeasurementsEdge,
  measurement,
  measurementCreateInput,
  measurementScoreSet,
  measurementsConnection,
  measurementsEdge,
  measurementsInDateRangeInput,
};

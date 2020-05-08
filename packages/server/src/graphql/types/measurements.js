const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const { MEASUREMENT_VALUE } = require('../../enums');
const resolvers = require('../resolvers');

const votesTypes = require('./votes');
const measurablesTypes = require('./measurables');
const agentsTypes = require('./agents');
const permissionsTypes = require('./permissions');
const commonTypes = require('./common');
const scalars = require('./scalars');

const { measurementUnresolvableResolution } = require('./enums');
const { measurementCommentType } = require('./enums');
const { measurementCompetitorType } = require('./enums');

const measurementValueInputFloatCdf = new graphql.GraphQLInputObjectType({
  name: 'MeasurementValueInputFloatCdf',
  fields: () => ({
    xs: {
      type: graphql.GraphQLNonNull(
        graphql.GraphQLList(
          graphql.GraphQLNonNull(graphql.GraphQLFloat),
        ),
      ),
    },
    ys: {
      type: graphql.GraphQLNonNull(
        graphql.GraphQLList(
          graphql.GraphQLNonNull(graphql.GraphQLFloat),
        ),
      ),
    },
  }),
});

const measurementValueFloatCdf = new graphql.GraphQLObjectType({
  name: 'MeasurementValueFloatCdf',
  fields: () => ({
    xs: {
      type: graphql.GraphQLNonNull(
        graphql.GraphQLList(
          graphql.GraphQLNonNull(graphql.GraphQLFloat),
        ),
      ),
    },
    ys: {
      type: graphql.GraphQLNonNull(
        graphql.GraphQLList(
          graphql.GraphQLNonNull(graphql.GraphQLFloat),
        ),
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
    id: { type: graphql.GraphQLNonNull(scalars.$measurementId) },
    value: {
      type: graphql.GraphQLNonNull(measurementValue),
      resolve: resolvers.measurements.value,
    },
    competitorType: {
      type: measurementCompetitorType,
    },
    description: { type: graphql.GraphQLString },
    measurableId: { type: graphql.GraphQLNonNull(scalars.$measurableId) },
    agentId: { type: scalars.$agentId },
    relevantAt: { type: DateType.default },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    cancelledAt: { type: DateType.default },
    taggedMeasurementId: { type: scalars.$measurementId },
    iAmOwner: commonTypes.iAmOwner,
    valueText: { type: graphql.GraphQLString },

    measurementScoreSet: {
      type: measurementScoreSet,
      resolve: resolvers.measurements.scoreSet,
    },

    totalVoteAmount: {
      type: graphql.GraphQLInt,
      resolve: resolvers.votes.total,
    },

    vote: {
      type: votesTypes.vote,
      resolve: resolvers.votes.oneByMeasurementId,
    },

    measurable: {
      type: measurablesTypes.measurable,
      resolve: resolvers.measurables.one,
    },

    agent: {
      type: agentsTypes.agent,
      resolve: resolvers.agents.one,
    },

    taggedMeasurement: {
      type: measurement,
      resolve: resolvers.measurements.one,
    },

    taggedBy: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(measurement)),
      resolve: resolvers.measurements.allByTaggedMeasurementId,
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
    cursor: { type: graphql.GraphQLNonNull(scalars.$cursor) },
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
    cursor: { type: graphql.GraphQLNonNull(scalars.$cursor) },
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
      resolve: resolvers.measurements.prediction,
    },
    outcome: {
      description: 'Returns latest objective measurement.',
      type: measurement,
      resolve: resolvers.measurements.outcome,
    },
    previousAggregate: {
      description: 'Returns latest aggregation measurement.',
      type: measurement,
      resolve: resolvers.measurements.previousAggregate,
    },
    nonMarketLogScore: {
      description: 'Not fully implemented yet.',
      type: graphql.GraphQLFloat,
      resolve: resolvers.measurements.nonMarketLogScore,
    },
    primaryPointScore: {
      description: 'Not fully implemented yet.',
      type: graphql.GraphQLFloat,
      resolve: resolvers.measurements.primaryPointScore,
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

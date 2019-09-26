const graphql = require('graphql');
const { resolver, DateType } = require('graphql-sequelize');

const models = require('../../models');
const resolvers = require('../resolvers');

const { measurableValueType } = require('./enums/measurable-value-type');
const { measurableState } = require('./enums/measurable-state');

const measurable = new graphql.GraphQLObjectType({
  name: 'Measurable',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    labelSubject: { type: graphql.GraphQLString },
    labelOnDate: { type: DateType.default },
    labelProperty: { type: graphql.GraphQLString },
    labelCustom: { type: graphql.GraphQLString },
    valueType: { type: measurableValueType },
    state: { type: graphql.GraphQLNonNull(measurableState) },
    stateUpdatedAt: { type: DateType.default },
    isArchived: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    resolutionEndpoint: { type: graphql.GraphQLString },
    expectedResolutionDate: { type: DateType.default },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },

    measurementCount: {
      type: graphql.GraphQLInt,
      resolve: resolvers.measurements.measurementCountByMeasurableId,
    },

    measurerCount: {
      type: graphql.GraphQLInt,
      resolve: resolvers.measurements.measurerCount,
    },

    resolutionEndpointResponse: { type: graphql.GraphQLFloat },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    creatorId: { type: graphql.GraphQLString },
    seriesId: { type: graphql.GraphQLString },
    iAmOwner: require('./common').iAmOwner,
    min: { type: graphql.GraphQLFloat },
    max: { type: graphql.GraphQLFloat },

    permissions: {
      type: graphql.GraphQLNonNull(require('./permissions').permissions),
      resolve: resolvers.permissions.measurablesPermissions,
    },

    Measurements: {
      type: require('./connections').measurableMeasurementsConnection.connectionType,
      args: require('./connections').measurableMeasurementsConnection.connectionArgs,
      resolve: require('./connections').measurableMeasurementsConnection.resolve,
    },

    series: {
      type: require('./series').series,
      resolve: resolver(models.Measurable.Series),
    },

    creator: {
      type: require('./agents').agent,
      resolve: resolver(models.Measurable.Creator),
    },

    Channel: {
      type: require('./channels').channel,
      resolve: resolver(models.Measurable.Channel),
    },

    recentMeasurement: {
      description: 'Returns either objective measurement for a judged'
        + ' measurable or latest measurement of an agent.',
      type: require('./measurements').measurement,
      resolve: resolvers.measurements.latest,
    },

    outcome: {
      description: 'Returns latest objective measurement.',
      type: require('./measurements').measurement,
      resolve: require('../resolvers/measurements').outcomeByRootId,
    },

    previousAggregate: {
      deprecated: 'Will be renamed on latestAggregate.',
      description: 'Returns latest aggregation measurement.',
      type: require('./measurements').measurement,
      resolve: require('../resolvers/measurements').latestAggregateByRootId,
    },
  }),
});

const measurableCreateInput = new graphql.GraphQLInputObjectType({
  name: 'MeasurableCreateInput',
  fields: () => ({
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    labelCustom: { type: graphql.GraphQLString },
    valueType: { type: require('./enums/measurable-value-type').measurableValueType },
    expectedResolutionDate: { type: DateType.default },
    resolutionEndpoint: { type: graphql.GraphQLString },
    labelSubject: { type: graphql.GraphQLString },
    labelOnDate: { type: DateType.default },
    labelProperty: { type: graphql.GraphQLString },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    min: { type: graphql.GraphQLFloat },
    max: { type: graphql.GraphQLFloat },
  }),
});

const measurableUpdateInput = new graphql.GraphQLInputObjectType({
  name: 'MeasurableUpdateInput',
  fields: () => ({
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    labelCustom: { type: graphql.GraphQLString },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    valueType: { type: require('./enums/measurable-value-type').measurableValueType },
    expectedResolutionDate: { type: DateType.default },
    resolutionEndpoint: { type: graphql.GraphQLString },
    labelSubject: { type: graphql.GraphQLString },
    labelOnDate: { type: DateType.default },
    labelProperty: { type: graphql.GraphQLString },
    min: { type: graphql.GraphQLFloat },
    max: { type: graphql.GraphQLFloat },
  }),
});

const measurablesEdge = new graphql.GraphQLObjectType({
  name: 'MeasurablesEdge',
  fields: () => ({
    node: { type: require('./measurables').measurable },
    cursor: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const measurablesConnection = new graphql.GraphQLObjectType({
  name: 'MeasurablesConnection',
  fields: () => ({
    total: { type: graphql.GraphQLInt },
    pageInfo: { type: graphql.GraphQLNonNull(require('./common').pageInfoConnection) },
    edges: { type: graphql.GraphQLList(require('./measurables').measurablesEdge) },
  }),
});

module.exports = {
  measurableState,
  measurable,
  measurableCreateInput,
  measurableUpdateInput,
  measurablesEdge,
  measurablesConnection,
};

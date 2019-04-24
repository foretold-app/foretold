const graphql = require('graphql');
const { resolver, DateType } = require('graphql-sequelize');

const models = require('../models');
const { MEASURABLE_STATE } = require('../models/measurable-state');
const resolvers = require('../resolvers');

const { valueType } = require('./value-type');

const measurableState = new graphql.GraphQLEnumType({
  name: 'measurableState',
  values: {
    [MEASURABLE_STATE.OPEN]: { value: MEASURABLE_STATE.OPEN },
    [MEASURABLE_STATE.JUDGED]: { value: MEASURABLE_STATE.JUDGED },
    [MEASURABLE_STATE.JUDGEMENT_PENDING]: { value: MEASURABLE_STATE.JUDGEMENT_PENDING },
  }
});

const measurable = new graphql.GraphQLObjectType({
  name: 'Measurable',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    labelSubject: { type: graphql.GraphQLString },
    labelOnDate: { type: DateType.default },
    labelProperty: { type: graphql.GraphQLString },
    labelCustom: { type: graphql.GraphQLString },
    valueType: { type: valueType },
    state: { type: graphql.GraphQLNonNull(measurableState) },
    stateUpdatedAt: { type: DateType.default },
    isArchived: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    resolutionEndpoint: { type: graphql.GraphQLString },
    expectedResolutionDate: { type: DateType.default },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    measurementCount: { type: graphql.GraphQLInt },
    measurerCount: { type: graphql.GraphQLInt },
    resolutionEndpointResponse: { type: graphql.GraphQLFloat },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    creatorId: { type: graphql.GraphQLString },
    seriesId: { type: graphql.GraphQLString },
    iAmOwner: require('./common').iAmOwner,

    permissions: {
      type: graphql.GraphQLNonNull(require('./permissions').permissions),
      resolve: resolvers.permissions.measurablesPermissions,
    },

    Measurements: {
      type: require('../connections').measurableMeasurementsConnection.connectionType,
      args: require('../connections').measurableMeasurementsConnection.connectionArgs,
      resolve: require('../connections').measurableMeasurementsConnection.resolve
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
  })
});

const measurableCreateInput = new graphql.GraphQLInputObjectType({
  name: 'MeasurableCreateInput',
  fields: () => ({
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    labelCustom: { type: graphql.GraphQLString },
    valueType: { type: require('./value-type').valueType },
    expectedResolutionDate: { type: DateType.default },
    resolutionEndpoint: { type: graphql.GraphQLString },
    labelSubject: { type: graphql.GraphQLString },
    labelOnDate: { type: DateType.default },
    labelProperty: { type: graphql.GraphQLString },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  })
});

const measurableUpdateInput = new graphql.GraphQLInputObjectType({
  name: 'MeasurableUpdateInput',
  fields: () => ({
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    labelCustom: { type: graphql.GraphQLString },
    expectedResolutionDate: { type: DateType.default },
    resolutionEndpoint: { type: graphql.GraphQLString },
    labelSubject: { type: graphql.GraphQLString },
    labelOnDate: { type: DateType.default },
    labelProperty: { type: graphql.GraphQLString },
  })
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

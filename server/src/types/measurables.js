const graphql = require('graphql');
const { resolver, DateType } = require('graphql-sequelize');

const models = require('../models');
const { MEASURABLE_STATE } = require('../models/measurable-state');

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
    descriptionEntity: { type: graphql.GraphQLString },
    descriptionDate: { type: DateType.default },
    descriptionProperty: { type: graphql.GraphQLString },
    description: { type: graphql.GraphQLString },
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

    Measurements: {
      type: graphql.GraphQLNonNull(new graphql.GraphQLList(require('./').measurementType)),
      resolve: resolver(models.Measurement),
    },

    series: {
      type: require('./').seriesType,
      resolve: resolver(models.Series),
    },

    creator: {
      type: require('./agents').agent,
      resolve: resolver(models.Agent),
    },

    Channel: {
      type: require('./channels').channel,
      resolve: resolver(models.Channel),
    },
  })
});

module.exports = {
  measurableState,
  measurable,
};

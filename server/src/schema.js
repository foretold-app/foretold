const _ = require('lodash');

const graphql = require("graphql");
const { attributeFields, resolver } = require("graphql-sequelize");

const models = require("./models");
const { measurementData, usersData, measurablesData, seriesData } = require('./data');
const data = require('./data');

const types = require('./types');
const { filterr } = require('./types/filterr');
const { stats } = require('./types/stats');

const schema = new graphql.GraphQLSchema({
  query: new graphql.GraphQLObjectType({
    name: 'Query',
    fields: {
      user: {
        type: types.userType,
        args: {
          id: { type: graphql.GraphQLString },
          auth0Id: { type: graphql.GraphQLString },
        },
        resolve: async (ops, values, options) => {
          return usersData.getUser(ops, values, options);
        }
      },
      users: {
        type: new graphql.GraphQLNonNull(graphql.GraphQLList(types.userType)),
        resolve: resolver(models.User),
      },

      measurement: {
        type: types.measurementType,
        args: _.pick(attributeFields(models.Measurement), ['id']),
        resolve: resolver(models.Measurement),
      },
      measurements: {
        type: new graphql.GraphQLNonNull(graphql.GraphQLList(types.measurementType)),
        resolve: resolver(models.Measurement),
      },

      measurable: {
        type: types.measurableType,
        args: _.pick(attributeFields(models.Measurable), ['id']),
        resolve: resolver(models.Measurable),
      },
      measurables: {
        type: new graphql.GraphQLNonNull(new graphql.GraphQLList(types.measurableType)),
        args: {
          offset: { type: graphql.GraphQLInt },
          limit: { type: graphql.GraphQLInt },
          creatorId: { type: graphql.GraphQLString },
          seriesId: { type: graphql.GraphQLString },
          channel: { type: graphql.GraphQLString }
        },
        resolve: async (ops, values, options) => {
          return measurablesData.getAll(ops, values, options);
        }
      },

      bot: {
        type: types.botType,
        args: _.pick(attributeFields(models.Bot), ['id']),
        resolve: resolver(models.Bot),
      },
      bots: {
        type: new graphql.GraphQLNonNull(graphql.GraphQLList(types.botType)),
        resolve: resolver(models.Bot),
      },

      agent: {
        type: types.agentType,
        args: _.pick(attributeFields(models.Agent), ['id']),
        resolve: resolver(models.Agent),
      },
      agents: {
        type: new graphql.GraphQLNonNull(graphql.GraphQLList(types.agentType)),
        resolve: resolver(models.Agent),
      },

      series: {
        type: types.seriesType,
        args: _.pick(attributeFields(models.Series), ['id']),
        resolve: resolver(models.Series),
      },
      seriesCollection: {
        type: new graphql.GraphQLNonNull(graphql.GraphQLList(types.seriesType)),
        resolve: resolver(models.Series),
      },

      channel: {
        type: types.channel,
        args: { id: { type: graphql.GraphQLString } },
        resolve: resolver(models.Channel),
      },
      channels: {
        type: new graphql.GraphQLNonNull(graphql.GraphQLList(types.channel)),
        resolve: resolver(models.Channel),
      },

      stats: {
        type: new graphql.GraphQLNonNull(stats),
        resolve: async (ops, values, options) => {
          // @todo:
          return 11;
        }
      },
    }
  }),
  mutation: new graphql.GraphQLObjectType({
    name: 'Mutation',
    fields: {
      createMeasurement: {
        type: types.measurementType,
        args: filterr(_.pick(attributeFields(models.Measurement), ['value', 'competitorType', 'measurableId', 'agentId', 'description'])),
        resolve: async (root, values, options) => {
          return measurementData.createMeasurement(root, values, options);
        },
      },
      createMeasurable: {
        type: types.measurableType,
        args: filterr(_.pick(attributeFields(models.Measurable), ['name', 'description', 'valueType', 'expectedResolutionDate', 'resolutionEndpoint', 'descriptionEntity', 'descriptionDate', 'descriptionProperty', 'channel'])),
        resolve: async (root, values, options) => {
          return measurablesData.createMeasurable(root, values, options);
        }
      },
      createSeries: {
        type: types.seriesType,
        args: filterr(_.pick(attributeFields(models.Series), ['name', 'description', 'channel', 'subjects', 'properties', 'dates'])),
        resolve: async (root, values, options) => {
          return seriesData.createSeries(root, values, options);
        }
      },
      archiveMeasurable: {
        type: types.measurableType,
        args: filterr(_.pick(attributeFields(models.Measurable), ['id'])),
        resolve: async (root, values, options) => {
          return measurablesData.archiveMeasurable(root, values, options);
        }
      },
      unArchiveMeasurable: {
        type: types.measurableType,
        args: filterr(_.pick(attributeFields(models.Measurable), ['id'])),
        resolve: async (root, values, options) => {
          return measurablesData.unArchiveMeasurable(root, values, options);
        }
      },
      editMeasurable: {
        type: types.measurableType,
        args: filterr(_.pick(attributeFields(models.Measurable), ['id', 'name', 'description', 'expectedResolutionDate', 'resolutionEndpoint', 'descriptionEntity', 'descriptionDate', 'descriptionProperty'])),
        resolve: async (root, values, options) => {
          return measurablesData.editMeasurable(root, values, options);
        }
      },
      editUser: {
        type: types.userType,
        args: filterr(_.pick(attributeFields(models.User), ["id", "name"])),
        resolve: async (root, values, options) => {
          return usersData.editUser(root, values, options);
        }
      },

      // @done
      channelUpdate: {
        type: types.channel,
        args: {
          id: { type: graphql.GraphQLString },
          input: { type: new graphql.GraphQLNonNull(types.channelInput) },
        },
        resolve: async (root, values) => {
          return data.channelsData.channelUpdate(root, values);
        },
      },

      // @done
      channelCreate: {
        type: types.channel,
        args: {
          input: { type: new graphql.GraphQLNonNull(types.channelInput) },
        },
        resolve: async (root, values, options) => {
          return data.channelsData.channelCreate(root, values, options);
        },
      },

      AddPersonToChannel: {
        type: types.channel,
        args: { id: { type: graphql.GraphQLString } },
        resolve: async (root, values, options) => {
          return data.channelsData.addPersonToChannel(root, values, options);
        },
      },

      RemovePersonFromChannel: {
        type: types.channel,
        args: { id: { type: graphql.GraphQLString } },
        resolve: async (root, values, options) => {
          return data.channelsData.removePersonFromChannel(root, values, options);
        },
      },

    }
  })
});

export {
  schema
};

const graphql = require('graphql');
const { createConnection } = require('graphql-sequelize');

const models = require('../../models');

const measurableMeasurementsConnection = createConnection({
  name: 'MeasurableMeasurements',
  nodeType: require('./measurements').measurement,
  target: models.Measurable.Measurements,
  connectionFields: {
    total: {
      type: graphql.GraphQLInt,
      resolve: ({ fullCount }) => fullCount,
    },
  },
});

const channelAgentsConnection = createConnection({
  name: 'ChannelAgents',
  nodeType: require('./agents').agent,
  target: models.Channel.Agents,
  connectionFields: {
    total: {
      type: graphql.GraphQLInt,
      resolve: ({ fullCount }) => fullCount,
    },
  },
});

const seriesMeasurablesConnection = createConnection({
  name: 'SeriesMeasurables',
  nodeType: require('./measurables').measurable,
  target: models.Series.Measurables,
  connectionFields: {
    total: {
      type: graphql.GraphQLInt,
      resolve: ({ fullCount }) => fullCount,
    },
  },
});

module.exports = {
  measurableMeasurementsConnection,
  channelAgentsConnection,
  seriesMeasurablesConnection,
};

const graphql = require('graphql');
const { resolver, JSONType, DateType } = require('graphql-sequelize');

const models = require('../models');

const measurementCreateInput = new graphql.GraphQLInputObjectType({
  name: 'MeasurementCreateInput',
  fields: () => ({
    value: { type: JSONType.default },
    competitorType: { type: require('./competitor').competitor },
    measurableId: { type: graphql.GraphQLString },
    agentId: { type: graphql.GraphQLString },
    description: { type: graphql.GraphQLString },
  }),
});

const measurement = new graphql.GraphQLObjectType({
  name: 'Measurement',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    value: { type: graphql.GraphQLNonNull(JSONType.default) },
    competitorType: { type: require('./competitor').competitor },
    description: { type: graphql.GraphQLString },
    measurableId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    agentId: { type: graphql.GraphQLString },
    relevantAt: { type: DateType.default },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    taggedMeasurementId: { type: graphql.GraphQLString },

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

module.exports = {
  measurement,
  measurementCreateInput,
};

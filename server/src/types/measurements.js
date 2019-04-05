const graphql = require('graphql');

const measurementCreateInput = new graphql.GraphQLInputObjectType({
  name: 'MeasurementCreateInput',
  fields: () => ({
    value: { type: require('graphql-sequelize').JSONType.default },
    competitorType: { type: require('./competitor').competitor },
    measurableId: { type: graphql.GraphQLString },
    agentId: { type: graphql.GraphQLString },
    description: { type: graphql.GraphQLString },
  }),
});

module.exports = {
  measurementCreateInput,
};

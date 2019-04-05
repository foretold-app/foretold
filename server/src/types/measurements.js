const graphql = require('graphql');
const { JSONType } = require('graphql-sequelize');

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

module.exports = {
  measurementCreateInput,
};

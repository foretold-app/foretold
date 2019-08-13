const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');
const { GraphQLJSON } = require('graphql-type-json');

const globalSetting = new graphql.GraphQLObjectType({
  name: 'GlobalSetting',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    entity: { type: GraphQLJSON },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    removedAt: { type: graphql.GraphQLNonNull(DateType.default) },
  })
});

module.exports = {
  globalSetting,
};

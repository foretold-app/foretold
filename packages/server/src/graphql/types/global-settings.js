const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');
const { GraphQLJSON } = require('graphql-type-json');

const globalSetting = new graphql.GraphQLObjectType({
  name: 'GlobalSetting',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    name: { type: graphql.GraphQLString },
    entityGraph: { type: GraphQLJSON },
    botAgentId: { type: graphql.GraphQLString },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
  }),
});

const globalSettingUpdateInput = new graphql.GraphQLInputObjectType({
  name: 'GlobalSettingUpdateInput',
  fields: () => ({
    botAgentId: { type: graphql.GraphQLString },
  }),
});

module.exports = {
  globalSetting,
  globalSettingUpdateInput,
};

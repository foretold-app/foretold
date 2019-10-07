const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');
const { GraphQLJSON } = require('graphql-type-json');

const { objectId } = require('./scalars');

const globalSetting = new graphql.GraphQLObjectType({
  name: 'GlobalSetting',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(objectId) },
    name: { type: graphql.GraphQLString },
    entityGraph: { type: GraphQLJSON },
    botAgentId: { type: objectId },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
  }),
});

const globalSettingUpdateInput = new graphql.GraphQLInputObjectType({
  name: 'GlobalSettingUpdateInput',
  fields: () => ({
    botAgentId: { type: objectId },
  }),
});

module.exports = {
  globalSetting,
  globalSettingUpdateInput,
};

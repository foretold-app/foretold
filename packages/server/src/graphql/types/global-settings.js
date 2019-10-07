const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');
const { GraphQLJSON } = require('graphql-type-json');

const { objectId, string256 } = require('./scalars');

const globalSetting = new graphql.GraphQLObjectType({
  name: 'GlobalSetting',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(objectId) },
    name: { type: string256 },
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

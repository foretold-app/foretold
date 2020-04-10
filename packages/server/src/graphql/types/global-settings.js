const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');
const { GraphQLJSON } = require('graphql-type-json');

const scalars = require('./scalars');

const globalSetting = new graphql.GraphQLObjectType({
  name: 'GlobalSetting',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(scalars.$globalSettingId) },
    name: { type: scalars.$globalSettingsName },
    entityGraph: { type: GraphQLJSON },
    botAgentId: { type: scalars.$agentId },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
  }),
});

const globalSettingUpdateInput = new graphql.GraphQLInputObjectType({
  name: 'GlobalSettingUpdateInput',
  fields: () => ({
    botAgentId: { type: scalars.$agentId },
  }),
});

module.exports = {
  globalSetting,
  globalSettingUpdateInput,
};

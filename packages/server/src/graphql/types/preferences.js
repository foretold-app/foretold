const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const { objectId } = require('./scalars');

const preference = new graphql.GraphQLObjectType({
  name: 'Preference',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(objectId) },
    agentId: { type: objectId },
    stopAllEmails: { type: graphql.GraphQLBoolean },
    enableExperimentalFeatures: {
      type: graphql.GraphQLNonNull(graphql.GraphQLBoolean),
    },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
  }),
});

const preferenceUpdateInput = new graphql.GraphQLInputObjectType({
  name: 'PreferenceUpdateInput',
  fields: () => ({
    stopAllEmails: { type: graphql.GraphQLBoolean },
    enableExperimentalFeatures: { type: graphql.GraphQLBoolean },
  }),
});

module.exports = {
  preference,
  preferenceUpdateInput,
};

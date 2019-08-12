const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const preference = new graphql.GraphQLObjectType({
  name: 'Preference',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    agentId: { type: graphql.GraphQLString },
    stopAllEmails: { type: graphql.GraphQLBoolean },
    enableExperimentalFeatures: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
  })
});

const preferenceUpdateInput = new graphql.GraphQLInputObjectType({
  name: 'PreferenceUpdateInput',
  fields: () => ({
    stopAllEmails: { type: graphql.GraphQLBoolean },
  }),
});

module.exports = {
  preference,
  preferenceUpdateInput,
};

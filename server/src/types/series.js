const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const seriesCreateInput = new graphql.GraphQLInputObjectType({
  name: 'SeriesCreateInput',
  fields: () => ({
    name: { type: graphql.GraphQLString },
    description: { type: graphql.GraphQLString },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    subjects: { type: graphql.GraphQLList(graphql.GraphQLString) },
    properties: { type: graphql.GraphQLList(graphql.GraphQLString) },
    dates: { type: graphql.GraphQLList(DateType.default) },
  })
});

module.exports = {
  seriesCreateInput,
};

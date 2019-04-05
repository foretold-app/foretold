const graphql = require('graphql');

const userUpdateInput =  new graphql.GraphQLInputObjectType({
  name: 'UserUpdateInput',
  fields: () => ({
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  })
});

module.exports = {
  userUpdateInput,
};

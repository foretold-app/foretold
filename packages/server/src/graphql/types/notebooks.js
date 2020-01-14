const graphql = require('graphql');
const { resolver, DateType } = require('graphql-sequelize');

const models = require('../../models');
const scalars = require('./scalars');

const notebook = new graphql.GraphQLObjectType({
  name: 'Notebook',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(scalars.notebookId) },
    // @entity: notebook-name-validation
    name: { type: graphql.GraphQLNonNull(scalars.string3to255) },
    // @entity: notebook-body-validation
    body: { type: graphql.GraphQLNonNull(scalars.string3to128K) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    // @entity: creator-link
    ownerId: { type: graphql.GraphQLNonNull(scalars.agentId) },
    channelId: { type: graphql.GraphQLNonNull(scalars.channelId) },

    // @todo: Do not use resolver. Use common interfaces of Data layer.
    owner: {
      type: graphql.GraphQLNonNull(require('./agents').agent),
      resolve: resolver(models.Notebook.Agent),
    },

    // @todo: Do not use resolver. Use common interfaces of Data layer.
    channel: {
      type: graphql.GraphQLNonNull(require('./channels').channel),
      resolve: resolver(models.Notebook.Channel),
    },
  }),
});

const notebookCreateInput = new graphql.GraphQLInputObjectType({
  name: 'NotebookCreateInput',
  fields: () => ({
    channelId: { type: graphql.GraphQLNonNull(scalars.channelId) },
    // @entity: notebook-name-validation
    name: { type: graphql.GraphQLNonNull(scalars.string3to255) },
    // @entity: notebook-body-validation
    body: { type: graphql.GraphQLNonNull(scalars.string3to128K) },
  }),
});

const notebookUpdateInput = new graphql.GraphQLInputObjectType({
  name: 'NotebookUpdateInput',
  fields: () => ({
    // @entity: notebook-name-validation
    name: { type: graphql.GraphQLNonNull(scalars.string3to255) },
    // @entity: notebook-body-validation
    body: { type: graphql.GraphQLNonNull(scalars.string3to128K) },
  }),
});


const notebookEdge = new graphql.GraphQLObjectType({
  name: 'NotebookEdge',
  fields: () => ({
    node: { type: require('./notebooks').notebook },
    cursor: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const notebooksConnection = new graphql.GraphQLObjectType({
  name: 'NotebooksConnection',
  fields: () => ({
    total: {
      type: graphql.GraphQLInt,
    },
    pageInfo: {
      type: graphql.GraphQLNonNull(require('./common').pageInfoConnection),
    },
    edges: {
      type: graphql.GraphQLList(require('./notebooks').notebookEdge),
    },
  }),
});

module.exports = {
  notebook,
  notebookCreateInput,
  notebookUpdateInput,
  notebookEdge,
  notebooksConnection,
};

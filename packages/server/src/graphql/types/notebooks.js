const graphql = require('graphql');
const { resolver, DateType } = require('graphql-sequelize');

const models = require('../../models/definitions');
const scalars = require('./scalars');

const commonTypes = require('./common');
const agentsTypes = require('./agents');
const channelsTypes = require('./channels');

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
      type: graphql.GraphQLNonNull(agentsTypes.agent),
      resolve: resolver(models.Notebook.Agent),
    },

    // @todo: Do not use resolver. Use common interfaces of Data layer.
    channel: {
      type: graphql.GraphQLNonNull(channelsTypes.channel),
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
    node: { type: notebook },
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
      type: graphql.GraphQLNonNull(commonTypes.pageInfoConnection),
    },
    edges: {
      type: graphql.GraphQLList(notebookEdge),
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

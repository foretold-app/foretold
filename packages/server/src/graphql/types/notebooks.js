const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const resolvers = require('../resolvers');

const scalars = require('./scalars');
const commonTypes = require('./common');
const agentsTypes = require('./agents');
const channelsTypes = require('./channels');
const permissionsTypes = require('./permissions');

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
    isBookmarked: { type: graphql.GraphQLBoolean },
    bookmarksCount: { type: graphql.GraphQLNonNull(graphql.GraphQLInt) },

    permissions: {
      type: graphql.GraphQLNonNull(permissionsTypes.permissions),
      resolve: resolvers.permissions.notebooksPermissions,
    },

    owner: {
      type: graphql.GraphQLNonNull(agentsTypes.agent),
      resolve: resolvers.agents.one,
    },

    channel: {
      type: graphql.GraphQLNonNull(channelsTypes.channel),
      resolve: resolvers.channels.one,
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


const orderFieldNotebook = new graphql.GraphQLEnumType({
  name: 'OrderFieldNotebook',
  values: {
    isBookmarked: { value: 'isBookmarked' },
  },
});

const orderNotebooks = new graphql.GraphQLInputObjectType({
  name: 'OrderNotebooks',
  fields: () => ({
    field: { type: graphql.GraphQLNonNull(orderFieldNotebook) },
    direction: {
      type: graphql.GraphQLNonNull(commonTypes.orderDirection),
    },
  }),
});

module.exports = {
  notebook,
  notebookCreateInput,
  notebookEdge,
  notebookUpdateInput,
  notebooksConnection,
  orderNotebooks,
};

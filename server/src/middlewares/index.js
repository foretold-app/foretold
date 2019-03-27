const { channel } = require('./channels');
const { agentsChannels } = require('./agents-channels');
const { measurable } = require('./measurables');
const { measurement } = require('./measurements');
const { series } = require('./series');

const middlewares = {
  Query: {

    // @tested
    measurements: async (resolve, root, args, context, info) => {
      await measurable(root, args, context, info);
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    // @tested
    measurement: async (resolve, root, args, context, info) => {
      await measurement(root, args, context, info);
      await measurable(root, args, context, info);
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    // @tested
    measurable: async (resolve, root, args, context, info) => {
      await measurable(root, args, context, info);
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    // @tested
    measurables: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    // @tested
    series: async (resolve, root, args, context, info) => {
      await series(root, args, context, info);
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    // @tested
    seriesCollection: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },
  },
};

module.exports = {
  middlewares,
};

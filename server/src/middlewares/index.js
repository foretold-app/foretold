const { channel } = require('./channels');
const { agentsChannels } = require('./agents-channels');
const { measurable } = require('./measurables');
const { measurement } = require('./measurements');
const { serie } = require('./series');

const middlewares = {
  Query: {
    measurements: async (resolve, root, args, context, info) => {
      await measurable(root, args, context, info);
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    measurement: async (resolve, root, args, context, info) => {
      await measurement(root, args, context, info);
      await measurable(root, args, context, info);
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    measurable: async (resolve, root, args, context, info) => {
      await measurable(root, args, context, info);
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    measurables: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    series: async (resolve, root, args, context, info) => {
      await serie(root, args, context, info);
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

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

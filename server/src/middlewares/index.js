const { channel } = require('./channels');
const { agentsChannels } = require('./agents-channels');
const { measurable } = require('./measurables');
const { measurement } = require('./measurements');
const { series } = require('./series');

const middlewares = {
  Query: {

    // Leave it for the next task
    measurements: async (resolve, root, args, context, info) => {
      await measurable(root, args, context, info);
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    // Leave it for the next task
    measurement: async (resolve, root, args, context, info) => {
      await measurement(root, args, context, info);
      await measurable(root, args, context, info);
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    // Leave it for the next task
    measurable: async (resolve, root, args, context, info) => {
      await measurable(root, args, context, info);
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    // Leave it for the next task
    measurables: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    // Leave it for the next task
    series: async (resolve, root, args, context, info) => {
      await series(root, args, context, info);
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    // Leave it for the next task
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

const { channel } = require('./channels');
const { agentsChannels } = require('./agents-channels');
const { measurable } = require('./measurables');

/**
 * Do not try to use DRY principle here.
 * Just read each section as it is.
 */

const middlewares = {
  Mutation: {
    createSeries: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    createMeasurement: async (resolve, root, args, context, info) => {
      await measurable(root, args, context, info);
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    channelUpdate: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    agentsChannelsCreate: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    agentsChannelsUpdate: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    agentsChannelsDelete: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await agentsChannels(root, args, context, info);
      return await resolve(root, args, context, info);
    },
  }
};

module.exports = {
  middlewares,
};

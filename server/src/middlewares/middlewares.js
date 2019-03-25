const { channel } = require('./channels');
const { agentsChannels } = require('./agents-channels');
const { measurable } = require('./measurables');

const middlewares = {
  Query: {
    measurements: async (resolve, parent, args, context, info) => {
      const result = await resolve(parent, args, context, info);
      await measurable(result, args, context, info);
      await channel(result, args, context, info);
      await agentsChannels(result, args, context, info);
      return result;
    },

    measurement: async (resolve, parent, args, context, info) => {
      const result = await resolve(parent, args, context, info);
      await measurable(result, args, context, info);
      await channel(result, args, context, info);
      await agentsChannels(result, args, context, info);
      return result;
    },

    measurable: async (resolve, parent, args, context, info) => {
      const result = await resolve(parent, args, context, info);
      await channel(result, args, context, info);
      await agentsChannels(result, args, context, info);
      return result;
    },

    measurables: async (resolve, parent, args, context, info) => {
      await channel(parent, args, context, info);
      await agentsChannels(parent, args, context, info);
      return await resolve(parent, args, context, info);
    },

    series: async (resolve, parent, args, context, info) => {
      const result = await resolve(parent, args, context, info);
      await channel(result, args, context, info);
      await agentsChannels(result, args, context, info);
      return result;
    },

    seriesCollection: async (resolve, parent, args, context, info) => {
      await channel(parent, args, context, info);
      await agentsChannels(parent, args, context, info);
      return await resolve(parent, args, context, info);
    },
  },
};

module.exports = {
  middlewares,
};

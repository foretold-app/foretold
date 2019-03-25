const { channel } = require('./channels');
const { agentsChannels } = require('./agents-channels');
const { measurable } = require('./measurables');
const { measurement } = require('./measurements');
const { serie } = require('./series');

const middlewares = {
  Query: {
    measurements: async (resolve, ...rest) => {
      await measurable(...rest);
      await channel(...rest);
      await agentsChannels(...rest);
      return await resolve(...rest);
    },

    measurement: async (resolve, ...rest) => {
      await measurement(...rest);
      await measurable(...rest);
      await channel(...rest);
      await agentsChannels(...rest);
      return await resolve(...rest);
    },

    measurable: async (resolve, ...rest) => {
      await measurable(...rest);
      await channel(...rest);
      await agentsChannels(...rest);
      return await resolve(...rest);
    },

    measurables: async (resolve, ...rest) => {
      await channel(...rest);
      await agentsChannels(...rest);
      return await resolve(...rest);
    },

    series: async (resolve, ...rest) => {
      await serie(...rest);
      await channel(...rest);
      await agentsChannels(...rest);
      return await resolve(...rest);
    },

    seriesCollection: async (resolve, ...rest) => {
      await channel(...rest);
      await agentsChannels(...rest);
      return await resolve(...rest);
    },
  },
};

module.exports = {
  middlewares,
};

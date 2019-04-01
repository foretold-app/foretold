module.exports = {
  up: async function (queryInterface) {
    await queryInterface.renameTable('AgentsChannels', 'ChannelsMemberships');
  },

  down: async function (queryInterface) {
    await queryInterface.renameTable('ChannelsMemberships', 'AgentsChannels',);
  }
};

module.exports = {
  up: async function (queryInterface) {
    await queryInterface.renameColumn(
      'Bots',
      'userId',
      'ownerUserId',
    );
    await queryInterface.renameColumn(
      'Bots',
      'agentId',
      'ownerAgentId',
    );
  },

  down: async function (queryInterface) {
    await queryInterface.renameColumn(
      'Bots',
      'ownerUserId',
      'userId',
    );
    await queryInterface.renameColumn(
      'Bots',
      'ownerAgentId',
      'agentId',
    );
  }
};

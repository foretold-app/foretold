module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      await queryInterface.addIndex('Invitations', ['agentId', 'channelId'], {
        name: 'Invitations_agentId_channelId_unique',
        unique: true,
      });
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      await queryInterface.removeIndex(
        'Invitations',
        'Invitations_agentId_channelId_unique',
      );
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};

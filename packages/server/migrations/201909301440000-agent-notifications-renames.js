module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      await queryInterface.renameTable(
        'AgentNotifications',
        'NotificationStatuses',
      );

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

      await queryInterface.renameTable(
        'NotificationStatuses',
        'AgentNotifications',
      );

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      await queryInterface.sequelize.query(
        `ALTER TABLE "AgentNotifications" ALTER COLUMN "agentId" DROP NOT NULL`
      );

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  },

  down: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      // @todo: To remove data with null column
      // @todo: or change it somehow.

      await queryInterface.sequelize.query(
        `ALTER TABLE "AgentNotifications" ALTER COLUMN "agentId" SET NOT NULL`
      );

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};

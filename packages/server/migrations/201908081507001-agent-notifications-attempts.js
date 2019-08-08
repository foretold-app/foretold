const {
    NOTIFICATION_ERROR_REASON,
} = require('../src/models/enums/notification-error-reason');

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      await queryInterface.addColumn('AgentNotifications', 'attemptCounter', {
        type: Sequelize.INTEGER,
        allowNull: false,
        defaultValue: 0,
      });

      await queryInterface.addColumn('AgentNotifications', 'errorAt', {
        type: Sequelize.DATE,
        allowNull: true,
      });

      await queryInterface.addColumn('AgentNotifications', 'errorReason', {
        type: Sequelize.ENUM([
          NOTIFICATION_ERROR_REASON.NONE,
          NOTIFICATION_ERROR_REASON.INTERNAL_ERROR,
          NOTIFICATION_ERROR_REASON.EXTERNAL_ERROR,
          NOTIFICATION_ERROR_REASON.PREFERENCES_ERROR,
          NOTIFICATION_ERROR_REASON.EMAIL_ADDRESS_ERROR,
        ]),
        defaultValue: NOTIFICATION_ERROR_REASON.NONE,
      });

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Up Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      await queryInterface.removeColumn('AgentNotifications', 'attemptCounter');
      await queryInterface.removeColumn('AgentNotifications', 'errorAt');
      await queryInterface.removeColumn('AgentNotifications', 'errorReason');
      await queryInterface.sequelize.query(`DROP TYPE "enum_AgentNotifications_errorReason"`);
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};

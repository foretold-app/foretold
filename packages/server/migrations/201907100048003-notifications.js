const { NOTIFICATION_TYPE } = require('../src/models/enums/notification-type');

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      await queryInterface.createTable('Notifications', {
        id: {
          allowNull: false,
          primaryKey: true,
          type: Sequelize.UUID
        },
        type: {
          type: Sequelize.ENUM([
            NOTIFICATION_TYPE.EMAIL,
            NOTIFICATION_TYPE.PUSH,
            NOTIFICATION_TYPE.WEB_PUSH,
          ]),
          defaultValue: NOTIFICATION_TYPE.EMAIL,
        },
        envelope: {
          type: Sequelize.JSON,
          allowNull: false,
        },
        createdAt: {
          type: Sequelize.DATE,
          defaultValue: Sequelize.NOW,
        },
        updatedAt: {
          type: Sequelize.DATE,
          defaultValue: Sequelize.NOW,
        },
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
      await queryInterface.dropTable('Notifications');
      await queryInterface.sequelize.query(`DROP TYPE "enum_Notifications_type"`);
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};

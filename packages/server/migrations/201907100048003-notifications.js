const { NOTIFICATION_TYPE } = require('../src/models/enums/notification-type');

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
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
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.dropTable('Notifications');
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};

const { INVITATION_STATUS } = require('../src/enums/invitation-status');

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      await queryInterface.createTable('Invitations', {
        id: {
          allowNull: false,
          primaryKey: true,
          type: Sequelize.UUID
        },
        email: {
          type: Sequelize.STRING,
          allowNull: false,
        },
        channelId: {
          type: Sequelize.UUID,
          references: {
            model: 'Channels',
            key: 'id',
          },
          allowNull: false,
        },
        status: {
          type: Sequelize.ENUM([
            INVITATION_STATUS.AWAITING,
            INVITATION_STATUS.ACCEPTED,
          ]),
          defaultValue: INVITATION_STATUS.ACCESS_TOKEN,
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

      await queryInterface.dropTable('Invitations');
      await queryInterface.sequelize.query(`DROP TYPE "enum_Invitations_status"`);

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};

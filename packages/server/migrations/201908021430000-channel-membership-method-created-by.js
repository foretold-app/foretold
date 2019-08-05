const {
  CHANNEL_MEMBERSHIP_TYPE,
} = require('../src/models/enums/channel-membership-type');

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      await queryInterface.addColumn('ChannelMemberships', 'methodCreatedBy', {
        type: Sequelize.ENUM([
          CHANNEL_MEMBERSHIP_TYPE.ADDED_IN_APP_BY_ADMIN,
          CHANNEL_MEMBERSHIP_TYPE.AGENT_JOINED_DIRECTLY,
          CHANNEL_MEMBERSHIP_TYPE.ADDED_BY_EMAIL_BY_ADMIN,
        ]),
        defaultValue: CHANNEL_MEMBERSHIP_TYPE.ADDED_IN_APP_BY_ADMIN,
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
      await queryInterface.removeColumn('ChannelMemberships', 'methodCreatedBy');
      await queryInterface.sequelize.query(`DROP TYPE "enum_ChannelMemberships_type"`);
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};

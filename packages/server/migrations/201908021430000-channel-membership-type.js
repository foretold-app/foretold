const { CHANNEL_MEMBERSHIP_TYPE } = require('../src/models/enums/channel-membership-roles');

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      await queryInterface.addColumn('ChannelMembership', 'type', {
        type: Sequelize.ENUM([
          CHANNEL_MEMBERSHIP_TYPE.ADDED,
          CHANNEL_MEMBERSHIP_TYPE.JOINED,
          CHANNEL_MEMBERSHIP_TYPE.INVITED,
        ]),
        defaultValue: CHANNEL_MEMBERSHIP_TYPE.ADDED,
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
      await queryInterface.removeColumn('ChannelMemberships', 'type');
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};

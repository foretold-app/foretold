const { TEMPLATE_NAME } = require('../src/enums');
const { FeedItem } = require('../src/models/classes');

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      const template = new FeedItem({
        item: '',
        description: '{{{ agent.name }}} joined the community',
      });

      await queryInterface.bulkInsert('Templates', [{
        id: Sequelize.fn('uuid_generate_v4'),
        envelopeTemplate: JSON.stringify(template),
        name: TEMPLATE_NAME.MEMBER_JOINED_COMMUNITY_FEED_ITEM,
        createdAt: new Date(),
        updatedAt: new Date(),
      }]);

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Up Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },

  down: async function (queryInterface) {

    try {
      await queryInterface.sequelize.query('BEGIN');

      await queryInterface.bulkDelete('Templates', {
        name: TEMPLATE_NAME.MEMBER_JOINED_COMMUNITY_FEED_ITEM,
      });

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  }
};

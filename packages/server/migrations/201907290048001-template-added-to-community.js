const { TEMPLATE_NAME } = require('../src/enums');
const { EmailEnvelopeTemplate } = require('../src/models/classes/templates');

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      const envelopeTemplate = new EmailEnvelopeTemplate({
        subject: 'You are added to the Foretold community',
        body:
          '<a href="{{{ inviterAgent.link }}}">{{{ inviterAgent.name }}}</a> '
          + 'has added you to the Foretold community, '
          + '[<a href="{{{ channel.link }}}">{{{ channel.name }}}</a>]'
      });

      await queryInterface.bulkInsert('Templates', [{
        id: Sequelize.fn('uuid_generate_v4'),
        envelopeTemplate: JSON.stringify(envelopeTemplate),
        name: TEMPLATE_NAME.MEMBER_ADDED_TO_COMMUNITY,
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
        name: TEMPLATE_NAME.MEMBER_ADDED_TO_COMMUNITY,
      });

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  }
};

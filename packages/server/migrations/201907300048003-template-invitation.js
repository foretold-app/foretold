const { TEMPLATE_NAME } = require('../src/models/enums/template-name');
const { EmailEnvelopeTemplate } = require('../src/models/classes/templates');

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      const envelopeTemplate = new EmailEnvelopeTemplate({
        subject: 'You are invited to the Foretold community',
        body:
          '<a href="{{{ inviterAgent.link }}}">{{ inviterAgent.name }}</a> has ' +
          'invited you to the Foretold community, ' +
          '[<a href="{{{ invitation.link }}}">click here to accept invitation</a>]'
      });

      await queryInterface.bulkInsert('Templates', [{
        id: Sequelize.fn('uuid_generate_v4'),
        envelopeTemplate: JSON.stringify(envelopeTemplate),
        name: TEMPLATE_NAME.MEMBER_INVITED_TO_COMMUNITY,
        createdAt: new Date(),
        updatedAt: new Date(),
      }]);

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

      await queryInterface.bulkDelete('Templates', {
        name: TEMPLATE_NAME.MEMBER_INVITED_TO_COMMUNITY,
      });

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};

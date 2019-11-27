const { TEMPLATE_NAME } = require('../src/enums');
const { EmailEnvelopeTemplate } = require('../src/models/classes/templates');

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      const envelopeTemplate = new EmailEnvelopeTemplate({
        subject: 'Question "{{{ measurable.name }}}" was resolved',
        body:
          'The question [{{{ measurable.name }}}] was resolved. ' +
          'Click <a href="{{{ measurable.link }}}">here</a> to see the result.',
      });

      await queryInterface.bulkInsert('Templates', [{
        id: Sequelize.fn('uuid_generate_v4'),
        envelopeTemplate: JSON.stringify(envelopeTemplate),
        name: TEMPLATE_NAME.MEASURABLE_STATE_IS_RESOLVED,
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
        name: TEMPLATE_NAME.MEASURABLE_STATE_IS_RESOLVED,
      });

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};

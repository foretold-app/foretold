const { TEMPLATE_NAME } = require('../src/models/enums/template-name');
const { EmailEnvelopeTemplate } = require('../src/models/classes/templates');

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      const envelopeTemplate  = new EmailEnvelopeTemplate({
        subject: 'Subject Example',
        body: 'Body Example',
      });

      await queryInterface.bulkInsert('Templates', [{
        id: Sequelize.fn('uuid_generate_v4'),
        envelopeTemplate: envelopeTemplate.toJSON(),
        name: TEMPLATE_NAME.MEASURABLE_STATE_IS_CHANGED,
        createdAt: new Date(),
        updatedAt: new Date(),
      }]);

    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {

      await queryInterface.bulkDelete('Templates', {
        name: TEMPLATE_NAME.MEASURABLE_STATE_IS_CHANGED,
      });

    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};

const { TEMPLATE_NAME } = require('../../src/enums');
const {
  EmailEnvelopeTemplate,
} = require('../../src/data/models/classes/templates');

module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');
      const envelopeTemplate = new EmailEnvelopeTemplate({
        subject: 'Question "{{{ measurable.name }}}" is changed',
        body: 'Your question, ' +
          '<a href="{{{ measurable.link }}}">{{{ measurable.name }}}</a>, ' +
          'has reached its expected ' +
          'resolution date. We recommend either resolving the question or ' +
          'moving the expected resolution date to another time. ' +
          'You can do that <a href="{{{ measurable.link }}}">here</a>.',
      });


      const values = {
        envelopeTemplate: JSON.stringify(envelopeTemplate),
      };

      const cond = {
        name: TEMPLATE_NAME.MEASURABLE_STATE_IS_CHANGED,
      };

      await queryInterface.bulkUpdate('Templates', values, cond);

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Up Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },

  down: async function (queryInterface) {
    // No need to make down migration,
    // it does not have a sense.
  }
};

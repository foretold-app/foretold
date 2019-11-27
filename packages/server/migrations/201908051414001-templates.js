const { TEMPLATE_NAME } = require('../src/enums');
const { FeedItem } = require('../src/models/classes');

const tpsl = [
  [
    TEMPLATE_NAME.NEW_MEASUREMENT_PREDICTION_FEED_ITEM,
    '{{{ agent.name }}} made a prediction',
  ], [
    TEMPLATE_NAME.NEW_MEASUREMENT_COMMENT_FEED_ITEM,
    '{{{ agent.name }}} made a new comment',
  ], [
    TEMPLATE_NAME.NEW_MEASUREMENT_RESOLUTION_FEED_ITEM,
    '{{{ agent.name }}} resolved the question with an answer',
  ], [
    TEMPLATE_NAME.NEW_MEASUREMENT_RESOLUTION_NOT_AVAILABLE_FEED_ITEM,
    '{{{ agent.name }}} has marked this question as unresolved',
  ], [
    TEMPLATE_NAME.MEASURABLE_REACHED_RESOLUTION_DATE_FEED_ITEM,
    `{{{ measurable.name }}} has reached it's expected resolution date`,
  ], [
    TEMPLATE_NAME.NEW_MEASURABLE_FEED_ITEM,
    '{{{ agent.name }}} made a new question',
  ],
];

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      for (let i = 0, max = tpsl.length; i < max; i++) {
        const name = tpsl[i][0];
        const description = tpsl[i][1];

        const template = new FeedItem({ description, item: '' });

        await queryInterface.bulkInsert('Templates', [{
          name,
          id: Sequelize.fn('uuid_generate_v4'),
          envelopeTemplate: JSON.stringify(template),
          createdAt: new Date(),
          updatedAt: new Date(),
        }]);
      }

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

      for (let i = 0, max = tpsl.length; i < max; i++) {
        const name = tpsl[i][0];

        await queryInterface.bulkDelete('Templates', { name });
      }

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};

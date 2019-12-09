const { MEASUREMENT_COMPETITOR_TYPE } = require('../../src/enums');

module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      const { COMPETITIVE, OBJECTIVE } = MEASUREMENT_COMPETITOR_TYPE;

      await queryInterface.sequelize.query(
        'UPDATE "Measurements" SET "relevantAt" = "createdAt" ' +
        `WHERE "competitorType" = '${COMPETITIVE}' ` +
        `OR "competitorType" = '${OBJECTIVE}'`,
      );

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

      // No Way Back

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};

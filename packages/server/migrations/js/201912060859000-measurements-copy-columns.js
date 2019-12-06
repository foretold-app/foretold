module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      // To catch a bug think as a bug!
      // Some rules:
      // 1. Copy all production DB or its part, or create data sample.
      // 2. Fix a code and DB.
      // 3. Try to find notable types of data as you can. And test it.

      // The epic of the ordering of aggregates and competitive measurements:
      // 1. We fixed the ordering of measurements in a code.
      // 2. We fixed method of getting a timestamp
      //    of measurement creation in a DB.
      // 3. We fixed a precision of timestamps in a DB.
      // 4. And now we want to copy "createdAt" timestamp of competitive
      //    measurement to "relevantAt" timestamp of aggregates.

      // This copies "createdAt" to "relevantAt" column.
      await queryInterface.sequelize.query(
        'UPDATE "Measurements" as "recipient" ' +
        'SET "relevantAt" = (' +
        'SELECT "createdAt" FROM "Measurements" as "donor" ' +
        'WHERE "donor"."id" = "recipient"."taggedMeasurementId" ' +
        'AND "donor"."competitorType"=\'COMPETITIVE\'' +
        ') WHERE "competitorType" = \'AGGREGATION\'',
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

      // No way back. See backups.

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};

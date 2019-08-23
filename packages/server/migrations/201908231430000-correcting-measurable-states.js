// If you feel that you want to import
// the enum with measurables' states then forget about this.
// Hence the application code and migration won't be
// in consistency anyway.
// So build DB schema separately.

const sql = (state) => `
  WITH "unresolvedMeasurements" AS (
      SELECT "id"
      FROM "Measurements"
      WHERE "competitorType" = 'UNRESOLVED'
      ORDER BY "createdAt" DESC
  )
  UPDATE "Measurables" SET "state" = '${state}'
  WHERE "id" in (SELECT "measurableId" FROM "unresolvedMeasurements")
`;

module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      await queryInterface.sequelize.query(sql('CLOSED_AS_UNRESOLVED'));
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration up error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      await queryInterface.sequelize.query(sql('UNRESOLVED'));
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration down error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};

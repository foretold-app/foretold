const table = 'Votes';
const timestampThree = 'timestamp(3) with time zone';
const timestampSix = 'timestamp(6) with time zone';

module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      await queryInterface.sequelize.query(
        `ALTER TABLE "${table}" `
        + `ALTER COLUMN "createdAt" SET DATA TYPE ${timestampThree}, `
        + `ALTER COLUMN "updatedAt" SET DATA TYPE ${timestampThree};`,
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

      await queryInterface.sequelize.query(
        `ALTER TABLE "${table}" `
        + `ALTER COLUMN "createdAt" SET DATA TYPE ${timestampSix}, `
        + `ALTER COLUMN "updatedAt" SET DATA TYPE ${timestampSix};`,
      );

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};

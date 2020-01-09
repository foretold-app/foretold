module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      await queryInterface.removeIndex(
        'Mutexes',
        'Mutexes_name_agentId_unique',
      );
      await queryInterface.removeIndex(
        'Mutexes',
        'Mutexes_name_unique',
      );

      await queryInterface.sequelize.query(
        'ALTER TABLE "Mutexes" ALTER COLUMN "agentId" SET NOT NULL',
      );

      // agentId = null
      await queryInterface.addIndex('Mutexes', ['name'], {
        name: 'Mutexes_name_unique',
        unique: true,
        where: {
          agentId: null,
        },
      });
      // agentId <> null
      await queryInterface.addIndex('Mutexes', ['name', 'agentId'], {
        name: 'Mutexes_name_agentId_unique',
        unique: true,
        where: {
          agentId: {
            [Sequelize.Op.ne]: null,
          },
        },
      });

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
        'DELETE FROM "Mutexes" WHERE "agentId" IS NULL',
      );

      await queryInterface.removeIndex(
        'Mutexes',
        'Mutexes_name_unique',
      );
      await queryInterface.removeIndex(
        'Mutexes',
        'Mutexes_name_agentId_unique',
      );

      await queryInterface.sequelize.query(
        'ALTER TABLE "Mutexes" ALTER COLUMN "agentId" DROP NOT NULL',
      );

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};

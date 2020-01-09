module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      await queryInterface.removeIndex(
        'Mutexes',
        'Mutexes_name_agentId_unique',
      );
      await queryInterface.removeConstraint(
        'Mutexes',
        'Mutexes_agentId_fkey',
      );

      await queryInterface.changeColumn('Mutexes', 'agentId', {
        type: Sequelize.UUID,
        allowNull: true,
      });
      await queryInterface.changeColumn('Mutexes', 'agentId', {
        type: Sequelize.UUID,
        references: {
          model: 'Agents',
          key: 'id',
        },
      });

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

  down: async function (queryInterface, Sequelize) {
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
      await queryInterface.removeConstraint(
        'Mutexes',
        'Mutexes_agentId_fkey',
      );

      await queryInterface.changeColumn('Mutexes', 'agentId', {
        type: Sequelize.UUID,
        references: {
          model: 'Agents',
          key: 'id',
        },
        allowNull: false,
      });
      await queryInterface.addIndex('Mutexes', ['name', 'agentId'], {
        name: 'Mutexes_name_agentId_unique',
        unique: true,
      });

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};

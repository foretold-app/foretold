module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      await queryInterface.createTable('Votes', {
        id: {
          allowNull: false,
          primaryKey: true,
          type: Sequelize.UUID,
        },
        measurementId: {
          type: Sequelize.UUID,
          references: {
            model: 'Measurements',
            key: 'id',
          },
          allowNull: false,
        },
        agentId: {
          type: Sequelize.UUID,
          references: {
            model: 'Agents',
            key: 'id',
          },
          allowNull: false,
        },
        voteAmount: {
          type: Sequelize.SMALLINT,
          allowNull: false,
          // And VoteAmount <> 0
          defaultValue: 0,
        },
        createdAt: {
          type: Sequelize.DATE,
          defaultValue: Sequelize.NOW,
          allowNull: false,
        },
        updatedAt: {
          type: Sequelize.DATE,
          defaultValue: Sequelize.NOW,
          allowNull: false,
        },
      });

      await queryInterface.addIndex('Votes', ['measurementId', 'agentId'], {
        name: 'Votes_measurementId_agentId_unique',
        unique: true,
      });

      // And VoteAmount <> 0
      await queryInterface.sequelize.query(
        'ALTER TABLE "Votes" ADD CONSTRAINT "voteAmount" '
        + 'CHECK ("voteAmount" >= -10 AND "voteAmount" <= 10);',
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
      await queryInterface.removeIndex(
        'Votes',
        'Votes_measurementId_agentId_unique',
      );
      await queryInterface.dropTable('Votes');
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};

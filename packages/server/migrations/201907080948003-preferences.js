module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');
      await queryInterface.createTable('Preferences', {
        id: {
          allowNull: false,
          primaryKey: true,
          type: Sequelize.UUID
        },
        agentId: {
          type: Sequelize.UUID,
          allowNull: false,
          references: {
            model: 'Agents',
            key: 'id',
          }
        },
        emails: {
          type: Sequelize.BOOLEAN,
          allowNull: false,
          defaultValue: true,
        },
        createdAt: {
          type: Sequelize.DATE,
          defaultValue: Sequelize.NOW,
        },
        updatedAt: {
          type: Sequelize.DATE,
          defaultValue: Sequelize.NOW,
        },
      });
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');
      await queryInterface.dropTable('Preferences');
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      await queryInterface.createTable('GlobalSettings', {
        id: {
          allowNull: false,
          primaryKey: true,
          type: Sequelize.UUID
        },
        name: {
          type: Sequelize.STRING,
          allowNull: false,
          unique: true,
        },
        entity: {
          type: Sequelize.JSON,
          allowNull: true,
        },
        createdAt: {
          type: Sequelize.DATE,
          defaultValue: Sequelize.NOW,
        },
        updatedAt: {
          type: Sequelize.DATE,
          defaultValue: Sequelize.NOW,
        },
        removedAt: {
          type: Sequelize.DATE,
          allowNull: true,
        },
      });

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
      await queryInterface.dropTable('GlobalSettings');
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};

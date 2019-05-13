module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      await queryInterface.createTable('Tokens', {
        id: {
          type: Sequelize.DataTypes.UUID(),
          primaryKey: true,
          defaultValue: Sequelize.fn('uuid_generate_v4'),
          allowNull: false,
        },
        agentId: {
          type: Sequelize.DataTypes.UUID(),
          allowNull: false,
          references: {
            model: 'Agents',
            key: 'id',
          }
        },
        token: {
          type: Sequelize.DataTypes.STRING,
          allowNull: true,
        },
        isActive: {
          type: Sequelize.DataTypes.BOOLEAN,
          allowNull: false,
          defaultValue: true,
        },
        createdAt: {
          type: Sequelize.DataTypes.DATE,
          defaultValue: Sequelize.DataTypes.NOW,
        },
        updatedAt: {
          type: Sequelize.DataTypes.DATE,
          defaultValue: Sequelize.DataTypes.NOW,
        },
      });

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      await queryInterface.sequelize.query(`ROLLBACK`);
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      await queryInterface.dropTable('Tokens');

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      await queryInterface.sequelize.query(`ROLLBACK`);
      console.error(e);
      throw e;
    }
  }
};


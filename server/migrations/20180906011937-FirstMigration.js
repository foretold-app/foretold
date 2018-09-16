'use strict';

module.exports = {
  up: async function (queryInterface, Sequelize) {
      var ID_TYPE = Sequelize.UUID;

      const ID = {
        allowNull: false,
        primaryKey: true,
        type: ID_TYPE
      };

      const referenceTo = (target, allowNull) => ({
        type: ID_TYPE,
        allowNull,
        references: {
          model: target,
          key: "id"
        }
      });

      const standardColumns = {
        id: ID,
        createdAt: {
          type: Sequelize.DATE
        },
        updatedAt: {
          type: Sequelize.DATE
        },
      };

      await queryInterface.createTable('Agents', {
        ...standardColumns,
        type: {
          allowNull: false,
          type: Sequelize.ENUM(["USER", "BOT"])
        },
      });

      await queryInterface.createTable('Users', {
        ...standardColumns,
        name: {
          allowNull: false,
          type: Sequelize.STRING
        },
        agentId: referenceTo("Agents", false),
      });

      await queryInterface.createTable('Bots', {
        ...standardColumns,
        name: {
          allowNull: true,
          type: Sequelize.STRING
        },
        description: {
          allowNull: true,
          type: Sequelize.TEXT
        },
        isNoncompetitive: {
          allowNull: false,
          defaultValue: false,
          type: Sequelize.BOOLEAN
        },
        userId: referenceTo("Users", false),
        agentId: referenceTo("Agents", false),
      });

      await queryInterface.createTable('Measurables', {
        ...standardColumns,
        name: {
          allowNull: false,
          type: Sequelize.STRING
        },
      });

      await queryInterface.createTable('Measurements', {
        ...standardColumns,
        percentile25: {
          allowNull: true,
          type: Sequelize.FLOAT
        },
        percentile50: {
          allowNull: true,
          type: Sequelize.FLOAT
        },
        percentile75: {
          allowNull: true,
          type: Sequelize.FLOAT
        },
        agentId: referenceTo("Agents", false),
        measurableId: referenceTo("Measurables", true),
        derivedById: referenceTo("Measurables", true),
      });
    },

    down: async function (queryInterface, Sequelize) {
      await queryInterface.dropTable('Measurements');
      await queryInterface.dropTable('Measureables');
      await queryInterface.dropTable('Users');
    }
};

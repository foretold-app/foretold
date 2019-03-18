module.exports = {
  up: async function (queryInterface, Sequelize) {
    await queryInterface.createTable('Channels', {
      id: {
        type: Sequelize.UUID,
        primaryKey: true,
        defaultValue: Sequelize.UUIDV4,
        allowNull: false,
      },
      name: {
        type: Sequelize.STRING,
        allowNull: false,
        unique: true,
      },
      description: {
        type: Sequelize.TEXT,
        allowNull: true,
      },
      isArchived: {
        type: Sequelize.BOOLEAN,
        allowNull: false,
        default: false,
      },
      isPublic: {
        type: Sequelize.BOOLEAN,
        allowNull: false,
        default: true,
      },
      creatorId: {
        type: Sequelize.UUID,
        allowNull: false,
        references: {
          model: 'Agents',
          key: 'id',
        }
      },
      createdAt: {
        type: Sequelize.DATE
      },
      updatedAt: {
        type: Sequelize.DATE
      },
    });
  },

  down: async function (queryInterface) {
    await queryInterface.dropTable('Channels');
  }
};


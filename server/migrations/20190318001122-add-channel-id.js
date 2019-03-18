module.exports = {
  up: async function (queryInterface) {
    await queryInterface.addColumn('Measurables', 'channelId', {
        type: Sequelize.UUID,
        allowNull: false,
        references: {
          model: 'Channel',
          key: 'id',
        }
      },
    );
    await queryInterface.addColumn('Series', 'channelId', {
        type: Sequelize.UUID,
        allowNull: false,
        references: {
          model: 'Channel',
          key: 'id',
        }
      },
    );
  },

  down: async function (queryInterface) {
    await queryInterface.removeColumn('Measurables', 'channelId');
    await queryInterface.removeColumn('Series', 'channelId');
  }
};


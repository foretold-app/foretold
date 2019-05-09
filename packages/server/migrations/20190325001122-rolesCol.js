module.exports = {
  up: async function (queryInterface, Sequelize) {
    await queryInterface.addColumn("AgentsChannels", "role", {
      type: Sequelize.STRING(8),
      allowNull: false,
      defaultValue: 'VIEWER'
    });
  },

  down: async function (queryInterface) {
    await queryInterface.removeColumn('AgentsChannels', 'role');
  }
};

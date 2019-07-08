module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
    await queryInterface.addColumn("AgentsChannels", "role", {
      type: Sequelize.STRING(8),
      allowNull: false,
      defaultValue: 'VIEWER'
    });
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
    await queryInterface.removeColumn('AgentsChannels', 'role');
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};

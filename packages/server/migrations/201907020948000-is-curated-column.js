module.exports = {
  up: async function (queryInterface, Sequelize) {
    await queryInterface.addColumn('Channels', 'isCurated', {
      type: Sequelize.BOOLEAN,
      allowNull: false,
      defaultValue: false,
    });
  },

  down: async function (queryInterface) {
    await queryInterface.removeColumn('Channels', 'isCurated');
  }
};

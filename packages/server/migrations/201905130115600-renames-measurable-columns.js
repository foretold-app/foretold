module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.addColumn('Agents', 'isAdmin', {
        type: Sequelize.BOOLEAN,
        allowNull: false,
        defaultValue: false,
      });
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.removeColumn('Agents', 'isAdmin');
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};

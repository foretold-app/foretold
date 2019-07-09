module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.addColumn('Users', 'isEmailVerified', {
        type: Sequelize.BOOLEAN,
        allowNull: true,
      });
    } catch (e) {
      console.error('Migration Up Error', e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.removeColumn('Users', 'isEmailVerified');
    } catch (e) {
      console.error('Migration Down Error', e);
      throw e;
    }
  }
};

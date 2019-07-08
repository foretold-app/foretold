module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      // Indexes
      await queryInterface.addIndex('Users', ['name'], {
        name: 'Users_name_unique',
        unique: true,
        where: {
          name: {
            [Sequelize.Op.ne]: "",
          },
        },
      });
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.removeIndex('Users', 'Users_name_unique');
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};

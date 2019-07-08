module.exports = {
  up: async function (queryInterface, Sequelize) {
    // Indexes
    await queryInterface.addIndex('Users', ['name'], {
      name: 'Users_name_unique',
      unique: true,
      where: {
        name: {
          [Sequelize.Op.ne]: null,
        },
      },
    });
  },

  down: async function (queryInterface) {
    await queryInterface.removeIndex('Users', 'Users_name_unique');
  }
};

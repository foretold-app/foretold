module.exports = {
  up: async function (queryInterface, Sequelize){
    await queryInterface.addColumn("Measurables", "descriptionEntity", {
          type: Sequelize.STRING,
          allowNull: true
    });
    await queryInterface.addColumn("Measurables", "descriptionDate", {
          type: Sequelize.DATE,
          allowNull: true, 
    });
  },

  down: async function (queryInterface, Sequelize){
      await queryInterface.removeColumn("Measurables", "descriptionEntity");
      await queryInterface.removeColumn("Measurables", "descriptionDate");
  }
};


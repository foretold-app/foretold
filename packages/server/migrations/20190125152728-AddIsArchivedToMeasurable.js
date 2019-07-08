module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
    await queryInterface.addColumn("Measurables", "isJudged", {
      type: Sequelize.BOOLEAN,
      allowNull: false,
      defaultValue: false,
    });
    await queryInterface.addColumn("Measurables", "state", {
      type: Sequelize.STRING,
      allowNull: false,
      defaultValue: "OPEN",
    });
    await queryInterface.addColumn("Measurables", "stateUpdatedAt", {
      type: Sequelize.DATE,
      allowNull: true,
    });
    await queryInterface.removeColumn("Measurables", "isLocked");
    await queryInterface.removeColumn("Measurables", "lockedAt");
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface, Sequelize) {
    try {
    await queryInterface.removeColumn("Measurables", "isJudged");
    await queryInterface.removeColumn("Measurables", "state");
    await queryInterface.removeColumn("Measurables", "stateUpdatedAt");
    await queryInterface.addColumn("Measurables", "isLocked", {
      allowNull: false,
      defaultValue: false,
      type: Sequelize.BOOLEAN
    });
    await queryInterface.addColumn("Measurables", "lockedAt", {
      allowNull: true,
      type: Sequelize.DATE
    });
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};


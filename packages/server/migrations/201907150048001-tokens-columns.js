const { TOKEN_TYPE } = require('../src/models/enums/token-type');

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {

      await queryInterface.addColumn('Tokens', 'expiresAt', {
        type: Sequelize.DATE,
        allowNull: true,
      });

      await queryInterface.addColumn('Tokens', 'type', {
        type: Sequelize.ENUM([
          TOKEN_TYPE.ACCESS_TOKEN,
          TOKEN_TYPE.AUTH_TOKEN,
        ]),
        defaultValue: TOKEN_TYPE.ACCESS_TOKEN,
      });

      await queryInterface.addColumn('Tokens', 'usageCount', {
        type: Sequelize.NUMBER,
        allowNull: true,
      });


    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    await queryInterface.removeColumn('Tokens', 'expiresAt');
    await queryInterface.removeColumn('Tokens', 'type');
    await queryInterface.removeColumn('Tokens', 'usageCount');
  }
};

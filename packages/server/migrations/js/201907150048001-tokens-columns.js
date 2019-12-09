const { TOKEN_TYPE } = require('../../src/enums');

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');

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
        type: Sequelize.INTEGER,
        allowNull: true,
      });

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Up Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');
      await queryInterface.removeColumn('Tokens', 'expiresAt');
      await queryInterface.removeColumn('Tokens', 'type');
      await queryInterface.removeColumn('Tokens', 'usageCount');
      await queryInterface.sequelize.query(`DROP TYPE "enum_Tokens_type"`);
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  }
};

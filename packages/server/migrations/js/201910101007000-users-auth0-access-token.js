module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');
      await queryInterface.addColumn('Users', 'auth0AccessToken', {
        type: Sequelize.STRING(64),
        allowNull: true,
      });
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');
      await queryInterface.removeColumn('Users', 'auth0AccessToken');
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  }
};

const { TOKEN_TYPE } = require('../src/enums/token-type');

module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      await queryInterface.addColumn('ChannelMemberships', 'inviterAgentId', {
        type: Sequelize.UUID,
        references: {
          model: 'Agents',
          key: 'id',
        },
        allowNull: true,
      });

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Up Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      await queryInterface.removeColumn('ChannelMemberships', 'inviterAgentId');
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};

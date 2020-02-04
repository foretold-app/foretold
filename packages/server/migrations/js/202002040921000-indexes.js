const fs = require('fs');
const path = require('path');

function sql(version = '') {
  const file = path.resolve(__dirname, `../resources/${version}.sql`);
  return fs.readFileSync(file, 'utf8');
}

// It is a temporary solution.
// Keywords: cache.

module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      // Drops views
      await queryInterface.sequelize.query(
        'DROP MATERIALIZED VIEW IF EXISTS "AgentChannels" CASCADE',
      );
      await queryInterface.sequelize.query(
        'DROP MATERIALIZED VIEW IF EXISTS "AgentMeasurable" CASCADE',
      );

      const agentChannels = sql('202002031534000-views/agent-channels');
      const agentMeasurables = sql('202002031534000-views/agent-measurables');

      // Creates views
      await queryInterface.sequelize.query(agentChannels);
      await queryInterface.sequelize.query(agentMeasurables);

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

      // Drops views
      await queryInterface.sequelize.query(
        'DROP MATERIALIZED VIEW IF EXISTS "AgentChannels" CASCADE',
      );
      await queryInterface.sequelize.query(
        'DROP MATERIALIZED VIEW IF EXISTS "AgentMeasurable" CASCADE',
      );

      const agentChannels = sql('202001131021000-views/agent-channels');
      const agentMeasurables = sql('202001131021000-views/agent-measurables');

      // Creates views
      await queryInterface.sequelize.query(agentChannels);
      await queryInterface.sequelize.query(agentMeasurables);

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};

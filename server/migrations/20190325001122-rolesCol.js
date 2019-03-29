module.exports = {
  up: async function (queryInterface) {
    await queryInterface.sequelize.query(`
        ALTER TABLE "AgentsChannels" ADD COLUMN "role" VARCHAR(8);
        UPDATE "AgentsChannels" SET "role" = 'ADMIN';
    `);
  },

  down: async function (queryInterface) {
    await queryInterface.removeColumn('AgentsChannels', 'role');
  }
};

module.exports = {
  up: async function (queryInterface) {
    await queryInterface.sequelize.query(`
        ALTER TABLE "AgentsChannels" ADD COLUMN "roles" VARCHAR(8)[];
        UPDATE "AgentsChannels" SET "roles" = ARRAY['viewer', 'admin'];
    `);
  },

  down: async function (queryInterface) {
    await queryInterface.removeColumn('AgentsChannels', 'roles');
  }
};

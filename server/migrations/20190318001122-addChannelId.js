const models = require('../src/models');

module.exports = {
  up: async function (queryInterface) {
    const channel = await models.Channel.findOne({
      where: { name: 'unlisted' },
    });

    await queryInterface.sequelize.query(`
        ALTER TABLE "Measurables" ADD COLUMN "channelId" uuid;
        UPDATE "Measurables" SET "channelId" = '${channel.id}';
        ALTER TABLE "Measurables" ALTER COLUMN "channelId" SET NOT NULL;
        
        ALTER TABLE "Measurables" ADD CONSTRAINT "Measurables_channelId_fkey" 
        FOREIGN KEY ("channelId") REFERENCES "Channels" ("id") 
        on update cascade on delete set null;
    `);

    await queryInterface.sequelize.query(`
        ALTER TABLE "Series" ADD COLUMN "channelId" uuid;
        UPDATE "Series" SET "channelId" = '${channel.id}';
        ALTER TABLE "Series" ALTER COLUMN "channelId" SET NOT NULL;

        ALTER TABLE "Series" ADD CONSTRAINT "Series_channelId_fkey"
        FOREIGN KEY ("channelId") REFERENCES "Channels" ("id")
        on update cascade on delete set null;
    `);
  },

  down: async function (queryInterface) {
    await queryInterface.removeColumn('Measurables', 'channelId');
    await queryInterface.removeColumn('Series', 'channelId');
  }
};

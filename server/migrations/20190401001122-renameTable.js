module.exports = {
  up: async function (queryInterface) {
    await queryInterface.renameTable('AgentsChannels', 'ChannelsMemberships');

    await queryInterface.sequelize.query(`
        ALTER TABLE "ChannelsMemberships" DROP CONSTRAINT "AgentsChannels_channelId_fkey";
        ALTER TABLE "ChannelsMemberships" DROP CONSTRAINT "AgentsChannels_agentId_fkey";
        
        ALTER TABLE "ChannelsMemberships" ADD CONSTRAINT "ChannelsMemberships_channelId_fkey" 
        FOREIGN KEY ("channelId") REFERENCES "Channels" ("id") 
        on update cascade on delete set null;
        
        ALTER TABLE "ChannelsMemberships" ADD CONSTRAINT "ChannelsMemberships_agentId_fkey" 
        FOREIGN KEY ("agentId") REFERENCES "Agents" ("id") 
        on update cascade on delete set null;
    `);
  },

  down: async function (queryInterface) {
    await queryInterface.renameTable('ChannelsMemberships', 'AgentsChannels');

    await queryInterface.sequelize.query(`
        ALTER TABLE "AgentsChannels" DROP CONSTRAINT "ChannelsMemberships_channelId_fkey";
        ALTER TABLE "AgentsChannels" DROP CONSTRAINT "ChannelsMemberships_agentId_fkey";
        
        ALTER TABLE "AgentsChannels" ADD CONSTRAINT "AgentsChannels_channelId_fkey" 
        FOREIGN KEY ("channelId") REFERENCES "Channels" ("id") 
        on update cascade on delete set null;
        
        ALTER TABLE "AgentsChannels" ADD CONSTRAINT "AgentsChannels_agentId_fkey" 
        FOREIGN KEY ("agentId") REFERENCES "Agents" ("id") 
        on update cascade on delete set null;
    `);
  }
};

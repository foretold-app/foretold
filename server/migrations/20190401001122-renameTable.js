module.exports = {
  up: async function (queryInterface) {
    await queryInterface.renameTable('AgentsChannels', 'ChannelMemberships');

    await queryInterface.sequelize.query(`
        ALTER TABLE "ChannelMemberships" DROP CONSTRAINT "AgentsChannels_channelId_fkey";
        ALTER TABLE "ChannelMemberships" DROP CONSTRAINT "AgentsChannels_agentId_fkey";
        
        ALTER TABLE "ChannelMemberships" ADD CONSTRAINT "ChannelMemberships_channelId_fkey" 
        FOREIGN KEY ("channelId") REFERENCES "Channels" ("id") 
        on update cascade on delete set null;
        
        ALTER TABLE "ChannelMemberships" ADD CONSTRAINT "ChannelMemberships_agentId_fkey" 
        FOREIGN KEY ("agentId") REFERENCES "Agents" ("id") 
        on update cascade on delete set null;
    `);
  },

  down: async function (queryInterface) {
    await queryInterface.renameTable('ChannelMemberships', 'AgentsChannels');

    await queryInterface.sequelize.query(`
        ALTER TABLE "AgentsChannels" DROP CONSTRAINT "ChannelMemberships_channelId_fkey";
        ALTER TABLE "AgentsChannels" DROP CONSTRAINT "ChannelMemberships_agentId_fkey";
        
        ALTER TABLE "AgentsChannels" ADD CONSTRAINT "AgentsChannels_channelId_fkey" 
        FOREIGN KEY ("channelId") REFERENCES "Channels" ("id") 
        on update cascade on delete set null;
        
        ALTER TABLE "AgentsChannels" ADD CONSTRAINT "AgentsChannels_agentId_fkey" 
        FOREIGN KEY ("agentId") REFERENCES "Agents" ("id") 
        on update cascade on delete set null;
    `);
  }
};

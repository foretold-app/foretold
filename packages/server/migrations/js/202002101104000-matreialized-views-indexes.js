const indexes = {
  "AgentChannels_id": ["AgentChannels", "id"],
  "AgentChannels_agentId": ["AgentChannels", "agentId"],
  "AgentChannels_channelId": ["AgentChannels", "channelId"],
  "AgentChannels_createdAt": ["AgentChannels", "createdAt"],
  "AgentChannels_updatedAt": ["AgentChannels", "updatedAt"],
  "AgentChannels_numberOfPredictions": ["AgentChannels", "numberOfPredictions"],
  "AgentChannels_numberOfQuestionsScored": ["AgentChannels", "numberOfQuestionsScored"],
  "AgentChannels_totalVotesReceived": ["AgentChannels", "totalVotesReceived"],

  "AgentMeasurables_id": ["AgentMeasurables", "id"],
  "AgentMeasurables_agentId": ["AgentMeasurables", "agentId"],
  "AgentMeasurables_measurableId": ["AgentMeasurables", "measurableId"],
  "AgentMeasurables_createdAt": ["AgentMeasurables", "createdAt"],
  "AgentMeasurables_updatedAt": ["AgentMeasurables", "updatedAt"],
  "AgentMeasurables_predictionCountTotal": ["AgentMeasurables", "predictionCountTotal"],
  "AgentMeasurables_totalVotesReceived": ["AgentMeasurables", "totalVotesReceived"],

  "ChannelAgents_agentId": ["ChannelAgents", "agentId"],
  "ChannelAgents_channelId": ["ChannelAgents", "channelId"],
  "ChannelAgents_createdAt": ["ChannelAgents", "createdAt"],
  "ChannelAgents_updatedAt": ["ChannelAgents", "updatedAt"],
};

/**
 * Here we could use "queryInterface.addIndex".
 */
module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      for (const indexName in indexes) {
        const index = indexes[indexName];
        const [table, column] = index;
        await queryInterface.addIndex(table, [column], {
          name: indexName,
        });
      }

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

      for (const indexName in indexes) {
        const index = indexes[indexName];
        const [table, _column] = index;
        await queryInterface.removeIndex(table, indexName);
      }

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};

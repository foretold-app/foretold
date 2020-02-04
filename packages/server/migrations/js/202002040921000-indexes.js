const indexes = {
  "Bots_agentId": ["Bots", "agentId"],
  "Bots_userId": ["Bots", "userId"],
  "ChannelMemberships_agentId": ["ChannelMemberships", "agentId"],
  "ChannelMemberships_channelId": ["ChannelMemberships", "channelId"],
  "Channels_creatorId": ["Channels", "creatorId"],
  "FeedItems_agentId": ["FeedItems", "agentId"],
  "FeedItems_channelId": ["FeedItems", "channelId"],
  "Measurables_channelId": ["Measurables", "channelId"],
  "Measurables_state": ["Measurables", "state"],
  "Measurements_agentId": ["Measurements", "agentId"],
  "Measurements_competitorType": ["Measurements", "competitorType"],
  "Measurements_measurableId": ["Measurements", "measurableId"],
  "Measurements_relevantAt": ["Measurements", "relevantAt"],
  "Measurements_taggedMeasurementId": ["Measurements", "taggedMeasurementId"],
  "Notebooks_channelId": ["Notebooks", "channelId"],
  "Notebooks_ownerId": ["Notebooks", "ownerId"],
  "NotificationStatuses_agentId": ["NotificationStatuses", "agentId"],
  "NotificationStatuses_notificationId": ["NotificationStatuses", "notificationId"],
  "NotificationStatuses_sentAt": ["NotificationStatuses", "sentAt"],
  "Notifications_type": ["Notifications", "type"],
  "Preferences_agentId": ["Preferences", "agentId"],
  "Tokens_agentId": ["Tokens", "agentId"],
  "Tokens_type": ["Tokens", "type"],
  "Users_agentId": ["Users", "agentId"],
  "Users_auth0Id": ["Users", "auth0Id"],
  "Users_email": ["Users", "email"],
  "Votes_measurementId": ["Votes", "measurementId"],
};

module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      for (const indexName in indexes) {
        const index = indexes[indexName];
        const [table, column] = index;
        await queryInterface.sequelize.query(
          `CREATE INDEX "${indexName}" `
          + `ON "${table}" ("${column}")`
        );
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
        await queryInterface.sequelize.query(`DROP INDEX "${indexName}"`);
      }

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};

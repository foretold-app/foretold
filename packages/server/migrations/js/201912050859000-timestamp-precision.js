const fs = require('fs');
const path = require('path');

const timestampThree = 'timestamp(3) with time zone';
const timestampSix = 'timestamp(6) with time zone';

const sqlAgentChannelsFile = path.resolve(
  __dirname,
  '../resources/201912050859000-views/agent-channels.sql',
);
const sqlAgentMeasurablesFile = path.resolve(
  __dirname,
  '../resources/201912050859000-views/agent-measurables.sql',
);
const sqlChannelAgentsFile = path.resolve(
  __dirname,
  '../resources/201912050859000-views/channel-agents.sql',
);

const sqlAgentChannels = fs.readFileSync(sqlAgentChannelsFile, 'utf8');
const sqlAgentMeasurables = fs.readFileSync(sqlAgentMeasurablesFile, 'utf8');
const sqlChannelAgents = fs.readFileSync(sqlChannelAgentsFile, 'utf8');

const tables = [
  'Agents', 'Bots', 'ChannelMemberships',
  'Channels', 'FeedItems', 'GlobalSettings',
  'Invitations', 'Measurables', 'Measurements', 'Mutexes',
  'Notebooks', 'Notifications', 'NotificationStatuses',
  'Preferences', 'Series', 'Templates', 'Tokens',
  'Users',
];

module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      // Drops views
      await queryInterface.sequelize.query(
        'DROP VIEW IF EXISTS "AgentChannels" CASCADE',
      );
      await queryInterface.sequelize.query(
        'DROP VIEW IF EXISTS "AgentMeasurable" CASCADE',
      );
      await queryInterface.sequelize.query(
        'DROP VIEW IF EXISTS "ChannelAgents" CASCADE',
      );

      // Alters tables
      for (const table of tables) {
        await queryInterface.sequelize.query(
          `ALTER TABLE "${table}" `
          + `ALTER COLUMN "createdAt" SET DATA TYPE ${timestampThree}, `
          + `ALTER COLUMN "updatedAt" SET DATA TYPE ${timestampThree};`,
        );
      }

      await queryInterface.sequelize.query(
          `ALTER TABLE "Measurables" `
        + `ALTER COLUMN "expectedResolutionDate" SET DATA TYPE ${timestampThree}, `
        + `ALTER COLUMN "stateUpdatedAt" SET DATA TYPE ${timestampThree}, `
        + `ALTER COLUMN "labelOnDate" SET DATA TYPE ${timestampThree};`,
      );

      await queryInterface.sequelize.query(
          `ALTER TABLE "NotificationStatuses" `
        + `ALTER COLUMN "sentAt" SET DATA TYPE ${timestampThree}, `
        + `ALTER COLUMN "errorAt" SET DATA TYPE ${timestampThree};`,
      );

      await queryInterface.sequelize.query(
          `ALTER TABLE "Measurements" `
        + `ALTER COLUMN "relevantAt" SET DATA TYPE ${timestampThree};`,
      );

      await queryInterface.sequelize.query(
          `ALTER TABLE "Tokens" `
        + `ALTER COLUMN "expiresAt" SET DATA TYPE ${timestampThree};`,
      );

      await queryInterface.sequelize.query(
          `ALTER TABLE "Series" `
        + `ALTER COLUMN "dates" SET DATA TYPE ${timestampThree}[];`,
      );

      // Creates views
      await queryInterface.sequelize.query(sqlChannelAgents);
      await queryInterface.sequelize.query(sqlAgentChannels);
      await queryInterface.sequelize.query(sqlAgentMeasurables);

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
        'DROP VIEW IF EXISTS "AgentChannels" CASCADE',
      );
      await queryInterface.sequelize.query(
        'DROP VIEW IF EXISTS "AgentMeasurable" CASCADE',
      );
      await queryInterface.sequelize.query(
        'DROP VIEW IF EXISTS "ChannelAgents" CASCADE;',
      );

      // Alters tables
      for (const table of tables) {
        await queryInterface.sequelize.query(
          `ALTER TABLE "${table}" `
          + `ALTER COLUMN "createdAt" SET DATA TYPE ${timestampSix}, `
          + `ALTER COLUMN "updatedAt" SET DATA TYPE ${timestampSix};`,
        );
      }

      await queryInterface.sequelize.query(
          `ALTER TABLE "Measurables" `
        + `ALTER COLUMN "expectedResolutionDate" SET DATA TYPE ${timestampSix}, `
        + `ALTER COLUMN "stateUpdatedAt" SET DATA TYPE ${timestampSix}, `
        + `ALTER COLUMN "labelOnDate" SET DATA TYPE ${timestampSix};`,
      );

      await queryInterface.sequelize.query(
          `ALTER TABLE "NotificationStatuses" `
        + `ALTER COLUMN "sentAt" SET DATA TYPE ${timestampSix}, `
        + `ALTER COLUMN "errorAt" SET DATA TYPE ${timestampSix};`,
      );

      await queryInterface.sequelize.query(
          `ALTER TABLE "Measurements" `
        + `ALTER COLUMN "relevantAt" SET DATA TYPE ${timestampSix};`,
      );

      await queryInterface.sequelize.query(
          `ALTER TABLE "Tokens" `
        + `ALTER COLUMN "expiresAt" SET DATA TYPE ${timestampSix};`,
      );

      // Creates views
      await queryInterface.sequelize.query(sqlChannelAgents);
      await queryInterface.sequelize.query(sqlAgentChannels);
      await queryInterface.sequelize.query(sqlAgentMeasurables);

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};

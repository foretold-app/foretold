module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      /**
       * It is a view but not an ordinary table.
       * There is no huge difference between a table
       * and a view for the application, expect the
       * "primaryPointScore" field (it is constant now).
       *
       * Why don't I want to create a table? Just imagine when
       * somebody creates a new question, guess what should I do
       * at the next step? Yes, I should add all members into this
       * table of a channel where that question is. For the first glance
       * not too bad. But what should I do if somebody adds new member into
       * a channel? Yes, I should create new rows: N - count of measurables,
       * M - count of members, so it is equal to N*M new rows. The same
       * will be for removing members, removing questions, removing channels.
       *
       * If we had added table we would have added DB triggers probably.
       *
       * Resuming at this moment:
       * --------------------------------
       * | Method                | Point |
       * |1. Table + triggers.   | 4     |
       * |2. View.               | 5     |
       * |3. Dynamic queries.    | 3     |
       * |4. Another solutions.  | 2     |
       * ---------------------------------
       *
       * Why are DB trigger bad? They are not bad but they are isolated on
       * a higher layer. So when DB changes a table (adds, removes rows) it
       * should notify the application to turn actions and events. It is messy.
       *
       * We just want to try future features before doing
       * much work.
       */
      await queryInterface.sequelize.query(`
        CREATE VIEW "AgentMeasurables" AS
          SELECT
            DISTINCT ON ("Measurables"."id", "ChannelMemberships"."agentId")
            uuid_generate_v4() AS "id",
            0.0 AS "primaryPointScore",
            "Measurables"."id" AS "measurableId",
            "ChannelMemberships"."agentId",
            "ChannelMemberships"."createdAt",
            "ChannelMemberships"."updatedAt",
            (SELECT count(*) FROM "Measurements"
                WHERE "Measurements"."measurableId"  = "Measurables" ."id"
            ) AS "predictionCountTotal"
          FROM "ChannelMemberships", "Measurables"
          WHERE "ChannelMemberships"."channelId" = "Measurables"."channelId";
      `);
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
      await queryInterface.sequelize.query('DROP VIEW "AgentMeasurables"');
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};

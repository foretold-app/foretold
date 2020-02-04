CREATE MATERIALIZED VIEW "AgentChannels" (
 "id", "agentId", "channelId", "primaryPointScore", "createdAt",
 "updatedAt", "numberOfPredictions", "numberOfQuestionsScored",
 "totalVotesReceived"
)

AS

SELECT uuid_generate_v4() AS id,
 "ChannelAgents"."agentId",
 "ChannelAgents"."channelId",
 0.0 AS "primaryPointScore",
 "ChannelAgents"."createdAt",
 "ChannelAgents"."updatedAt",

 (
 SELECT count(DISTINCT "Measurements".id) AS count
  FROM "Measurements"
    LEFT JOIN "Measurables"
      ON "Measurables".id = "Measurements"."measurableId"
  WHERE "Measurables"."channelId" = "ChannelAgents"."channelId"
    AND "Measurements"."agentId" = "ChannelAgents"."agentId"
    AND ("Measurements"."competitorType" = ANY
         (ARRAY ['OBJECTIVE'::"enum_Measurements_competitorType",
           'COMPETITIVE'::"enum_Measurements_competitorType"]))
 ) AS "numberOfPredictions",

 (
 SELECT count(DISTINCT "Measurables".id) AS count
  FROM "Measurables", "Measurements"
  WHERE "Measurables".id = "Measurements"."measurableId"
    AND "Measurables"."channelId" = "ChannelAgents"."channelId"
    AND "Measurements"."agentId" = "ChannelAgents"."agentId"
    AND ("Measurements"."competitorType" = ANY
         (ARRAY ['OBJECTIVE'::"enum_Measurements_competitorType",
             'COMPETITIVE'::"enum_Measurements_competitorType"]))
    AND "Measurables".state::text = 'JUDGED'::text
  ) AS "numberOfQuestionsScored",

 (
 SELECT SUM("Votes"."voteAmount") as "sum"
   FROM "Votes", "Measurements", "Measurables"
   WHERE "Measurables"."channelId" = "ChannelAgents"."channelId"
    AND "Measurements"."agentId" = "ChannelAgents"."agentId"
    AND "Measurements"."id" = "Votes"."measurementId"
    AND "Measurables"."id" = "Measurements"."measurableId"
 ) AS "totalVotesReceived"

FROM "ChannelAgents"

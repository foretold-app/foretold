CREATE MATERIALIZED VIEW "AgentMeasurables" (
  "id", "primaryPointScore", "measurableId", "agentId", "createdAt",
   "updatedAt", "predictionCountTotal", "timeAverageScore", "totalVotes"
)

AS

SELECT
 DISTINCT ON ("Measurables".id, "ChannelAgents"."agentId")
 uuid_generate_v4() AS id,
 0.0 AS "primaryPointScore",
 "Measurables"."id" AS "measurableId",
 "ChannelAgents"."agentId",
 "ChannelAgents"."createdAt",
 "ChannelAgents"."updatedAt",
 (SELECT count(*) AS count
  FROM "Measurements"
  WHERE "Measurements"."measurableId" =
        "Measurables".id
    AND "Measurements"."agentId" =
        "ChannelAgents"."agentId"
    AND ("Measurements"."competitorType" = ANY
         (ARRAY ['OBJECTIVE'::"enum_Measurements_competitorType",
         'COMPETITIVE'::"enum_Measurements_competitorType"])))
 AS "predictionCountTotal",
 0.0 AS "timeAverageScore",
 (
     SELECT SUM("Votes"."voteAmount") as "sum"
     FROM "Votes", "Measurements", "Measurables"
     WHERE "Measurables"."channelId" = "ChannelAgents"."channelId"
       AND "Measurements"."agentId" = "ChannelAgents"."agentId"
       AND "Measurements"."id" = "Votes"."measurementId"
       AND "Measurables"."id" = "Measurements"."measurableId"
 ) AS "totalVotes"
FROM "ChannelAgents", "Measurables"
WHERE "ChannelAgents"."channelId" = "Measurables"."channelId"

CREATE MATERIALIZED VIEW "AgentMeasurables" (
  "id", "primaryPointScore", "measurableId", "agentId", "createdAt",
   "updatedAt", "predictionCountTotal", "timeAverageScore"
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
 0.0 AS "timeAverageScore"
FROM "ChannelAgents", "Measurables"
WHERE "ChannelAgents"."channelId" = "Measurables"."channelId"

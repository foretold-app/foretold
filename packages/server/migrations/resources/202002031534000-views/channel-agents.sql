CREATE MATERIALIZED VIEW "ChannelAgents" (
  "channelId",
  "agentId",
  "createdAt",
  "updatedAt"
)

AS

WITH "MembersFromMeasurements" AS (
    SELECT DISTINCT "Measurables"."channelId",
                    "Measurements"."agentId",
                    "Measurements"."createdAt",
                    "Measurements"."updatedAt"
    FROM "Measurables",
         "Measurements"
    WHERE "Measurements"."measurableId" = "Measurables".id
      AND ("Measurements"."competitorType" = ANY
           (ARRAY ['COMPETITIVE'::"enum_Measurements_competitorType",
           'OBJECTIVE'::"enum_Measurements_competitorType"]))
),
"MembersAsMembers" AS (
   SELECT "ChannelMemberships"."agentId",
          "ChannelMemberships"."channelId",
          "ChannelMemberships"."createdAt",
          "ChannelMemberships"."updatedAt"
   FROM "ChannelMemberships"
)

SELECT DISTINCT ON ("ChannelAgents2"."channelId", "ChannelAgents2"."agentId")
 "ChannelAgents2"."channelId",
 "ChannelAgents2"."agentId",
 "ChannelAgents2"."createdAt",
 "ChannelAgents2"."updatedAt"
FROM (
  SELECT "MembersFromMeasurements"."channelId",
         "MembersFromMeasurements"."agentId",
         "MembersFromMeasurements"."createdAt",
         "MembersFromMeasurements"."updatedAt"
  FROM "MembersFromMeasurements"
  UNION
  SELECT "MembersAsMembers"."agentId",
         "MembersAsMembers"."channelId",
         "MembersAsMembers"."createdAt",
         "MembersAsMembers"."updatedAt"
  FROM "MembersAsMembers"
) "ChannelAgents2";

const _ = require('lodash');
const assert = require('assert');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class AgentMeasurableModel extends ModelPostgres {

  constructor() {
    super({
      model: models.AgentMeasurable,
      sequelize: models.sequelize,
    });
  }

  /**
   * @param {Models.ObjectID} agentId
   * @param {Models.ObjectID} measurableId
   * @param {string} [name]
   * @return {{
   *   id: string,
   *   agentId: string,
   *   createdAt: string,
   *   measurableCreatedAt: string,
   *   aggregations: {id: string, createdAt: string, value: *, type: string}[]
   *   previousAggregation: {id: string, createdAt: string, value: *, type: string},
   *   recentResult: {id: string, createdAt: string, value: *, type: string},
   * }}
   */
  async getMeasurementsForScoring(agentId, measurableId, name = '') {
    const query = this._measurementsForScoring(agentId, measurableId);
    const response = await this.sequelize.query(query);
    return _.head(response);
  }

  /**
   * @param {Models.ObjectID} agentId
   * @param {Models.ObjectID} measurableId
   * @param {string} [name]
   * @return {string}
   */
  _measurementsForScoring(agentId, measurableId, name = '') {
    assert(!!agentId, 'Agent ID is required.');
    assert(!!measurableId, 'Measurable ID is required.');
    return `(
      /* Agent-Measurable Scoring (${ name }) */
      SELECT
        /* all of the aggregates made after that */
        (
          SELECT array_to_json(
            ARRAY(
              SELECT json_build_object(
                'id', "Measurements2"."id",
                'createdAt', "Measurements2"."createdAt",
                'value', ("Measurements2"."value" ->> 'data')::json,
                'type', "Measurements2"."value" ->> 'dataType'
              )
              FROM "Measurements" AS "Measurements2"
              WHERE "Measurements2"."competitorType" = 'AGGREGATION'
                AND "Measurements2"."measurableId" = "Measurements"."measurableId"
                AND "Measurements2"."createdAt" > "Measurements"."createdAt"
              ORDER BY "Measurements2"."createdAt" DESC
            )
          )
        ) AS "aggregations",
    
        /* and the aggregate made immediately before that, with timestamps */
        (
          SELECT json_build_object(
            'id', "Measurements3"."id",
            'createdAt', "Measurements3"."createdAt",
            'value', ("Measurements3"."value" ->> 'data')::json,
            'type', "Measurements3"."value" ->> 'dataType'
          )
          FROM "Measurements" AS "Measurements3"
          WHERE "Measurements3"."competitorType" = 'AGGREGATION'
            AND "Measurements3"."measurableId" = "Measurements"."measurableId"
            AND "Measurements3"."createdAt" < "Measurements"."createdAt"
          ORDER BY "Measurements3"."createdAt" DESC
          LIMIT 1
         ) AS "previousAggregation",
    
        /* the most recent result, with a timestamp */
        (
          SELECT json_build_object(
              'id', "Measurements4"."id",
              'createdAt', "Measurements4"."createdAt",
              'value', ("Measurements4"."value" ->> 'data')::json,
              'type', "Measurements4"."value" ->> 'dataType'
            )
          FROM "Measurements" AS "Measurements4"
          WHERE "Measurements4"."competitorType" = 'OBJECTIVE'
            AND "Measurements4"."measurableId" = "Measurements"."measurableId"
          ORDER BY "Measurements4"."createdAt" DESC
          LIMIT 1
        ) AS "recentResult",
        
        "Measurements"."id",
        "Measurements"."agentId",
        "Measurements"."createdAt",
    
        /* the timestamp of when the measurable was created */
        "Measurables"."createdAt" AS "measurableCreatedAt"
      
      FROM "Measurements"
      LEFT JOIN "Measurables"
      ON "Measurables"."id" = "Measurements"."measurableId"
      
      WHERE "Measurements"."competitorType" = 'COMPETITIVE'
        AND "Measurements"."agentId" = '${ agentId }'
        AND "Measurements"."measurableId" = '${ measurableId }'
      ORDER BY "Measurements"."createdAt" DESC
    )`;
  }
}

module.exports = {
  AgentMeasurableModel,
};

const _ = require('lodash');

const models = require('../models');
const { splitBy } = require('../lib/functions');

const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class MeasurementModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Measurement,
      sequelize: models.sequelize,
    });
  }

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.filter} [filter]
   * @param {string} [filter.measurableId]
   * @param {string} [filter.agentId]
   * @param {string[]} [filter.competitorType]
   * @param {string} [filter.notTaggedByAgent]
   * @param {Layers.AbstractModelsLayer.pagination} [pagination]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @return {Promise<{data: Models.Measurement[], total: number}>}
   */
  async getAll(filter, pagination, restrictions) {
    const startDate = _.get(filter, 'findInDateRange.startDate');
    const endDate = _.get(filter, 'findInDateRange.endDate');
    const spacedLimit = _.get(filter, 'findInDateRange.spacedLimit');

    const where = {};
    const include = [];

    this.applyRestrictions(where, restrictions);

    if (filter.measurableId) where.measurableId = filter.measurableId;
    if (filter.agentId) where.agentId = filter.agentId;
    if (filter.competitorType) where.competitorType = {
      [this.in]: filter.competitorType,
    };
    if (startDate) _.set(where, ['createdAt', this.gte], startDate);
    if (endDate) _.set(where, ['createdAt', this.lte], endDate);
    if (filter.notTaggedByAgent) include.push({
      model: this.models.Measurement,
      as: 'UntaggedMeasurement',
      where: { agentId: filter.notTaggedByAgent, },
      on: {
        '$Measurement.id$': {
          [this.not]: this.col('Measurement.taggedMeasurementId'),
        },
      },
    });

    /** @type {number} */
    const total = await this.model.count({ where, include });
    const edgePagination = this.getPagination(pagination, total);

    const cond = {
      limit: edgePagination.limit,
      offset: edgePagination.offset,
      order: [['createdAt', 'DESC']],
      where,
      include,
    };

    /** @type {Models.Measurement[]} */
    let data = await this.model.findAll(cond);
    data = this.setIndexes(data, edgePagination);
    data.total = total;

    // tricky
    if (spacedLimit) data = splitBy(data, spacedLimit);

    return { data, total };
  }
}

module.exports = {
  MeasurementModel,
};

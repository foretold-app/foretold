const { DataBase } = require('./data-base');

const { SeriesModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {SeriesModel} SeriesModel
 */
class SeriesData extends DataBase {

  constructor() {
    super();
    this.SeriesModel = new SeriesModel();
    this.model = this.SeriesModel;
  }

  /**
   * @public
   * @deprecated: use createdOne
   * @param {object} data
   * @return {Promise<*>}
   */
  async createOne(data) {
    return this.super.createOne(data);
  }

  /**
   * @todo: move down
   * @todo: fix interface (filter, pagination, options*)
   * @public
   * @deprecated: use getAll
   * @param {object} [filter]
   * @param {Models.ObjectID} [filter.agentId]
   * @param {object} [_pagination]
   * @param {object} [_options]
   * @return {Promise<*>}
   */
  async getAll(filter = {}, _pagination = {}, _options = {}) {
    const { channelId } = filter;

    let where = {
      // Restrictions
      channelId: {
        [this.model.Op.in]: this.SeriesModel._channelIdsLiteral(filter.agentId),
      },
    };

    if (channelId) {
      // @todo: It breaks restrictions.
      where.channelId = {
        [this.model.Op.eq]: channelId,
      };
    }

    return this.models.Series.findAll({ where });
  }

  /**
   * @todo: move down
   * @todo: fix interface (params, query, options)
   * @public
   * @deprecated: use getOne
   * @param {Models.ObjectID} id
   * @param {object} [options]
   * @param {Models.ObjectID} [options.agentId]
   * @return {Promise<*>}
   */
  async getOne(id, options = {}) {
    return this.models.Series.findOne({
      where: {
        id,
        channelId: {
          [this.model.Op.in]: this.SeriesModel._channelIdsLiteral(options.agentId),
        },
      },
    });
  }
}

module.exports = {
  SeriesData,
};

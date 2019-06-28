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
   * @param {object} data
   * @return {Promise<*>}
   */
  async createOne(data) {
    return this.models.Series.create(data);
  }

  /**
   * @todo: move down
   * @todo: fix interface (filter, pagination, options*)
   * @param {object} options
   * @param {Models.ObjectID} [options.agentId]
   * @return {Promise<*>}
   */
  async getAll(options) {
    const { channelId } = options;

    let where = {
      // Restrictions
      channelId: {
        [this.model.Op.in]: this.SeriesModel.channelIdsLiteral(options.agentId),
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
   * @param {Models.ObjectID} id
   * @param {object} options
   * @param {Models.ObjectID} [options.agentId]
   * @return {Promise<*>}
   */
  async getOne(id, options = {}) {
    return this.models.Series.findOne({
      where: {
        id,
        channelId: {
          [this.model.Op.in]: this.SeriesModel.channelIdsLiteral(options.agentId),
        },
      },
    });
  }
}

module.exports = {
  SeriesData,
};

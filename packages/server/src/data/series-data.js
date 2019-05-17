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
    return await this.models.Series.create(data);
  }

  /**
   * @todo: move down
   * @todo: fix interface (filter, pagination, options*)
   * @param {object} options
   * @param {string} [options.agentId]
   * @return {Promise<*>}
   */
  async getAll(options) {
    const { channelId } = options;

    let where = {
      // Restrictions
      channelId: {
        [this.models.sequelize.Op.in]: this.SeriesModel.channelIdsLiteral(options.agentId)
      },
    };

    if (channelId) {
      // @todo: It breaks restrictions.
      where.channelId = { [this.models.sequelize.Op.eq]: channelId };
    }

    return await this.models.Series.findAll({ where });
  }

  /**
   * @todo: move down
   * @todo: fix interface (params, query, options)
   * @param {string} id
   * @param {object} options
   * @param {string} [options.agentId]
   * @return {Promise<*>}
   */
  async getOne(id, options = {}) {
    return await this.models.Series.findOne({
      where: {
        id,
        channelId: {
          [this.models.sequelize.Op.in]: this.SeriesModel.channelIdsLiteral(options.agentId)
        },
      },
    });
  }
}

module.exports = {
  SeriesData,
};

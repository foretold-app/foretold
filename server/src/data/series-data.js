const models = require("../models");

const { DataBase } = require('./data-base');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 */
class SeriesData extends DataBase {

  /**
   * @param {object} data
   * @return {Promise<*>}
   */
  async createOne(data) {
    return await models.Series.create(data);
  }

  /**
   * @param {object} options
   * @param {string} [options.agentId]
   * @return {Promise<*>}
   */
  async getAll(options) {
    const { channelId } = options;

    let where = {
        channelId: { $in: this.channelIdsLiteral(options.agentId) },
      };

    if (channelId) {
      where.channelId = { [models.sequelize.Op.eq]: channelId };
    }

    return await models.Series.findAll({
      where
    });
  }

  /**
   * @param {string} id
   * @param {object} options
   * @param {string} [options.agentId]
   * @return {Promise<*>}
   */
  async getOne(id, options = {}) {
    return await models.Series.findOne({
      where: {
        id,
        channelId: { $in: this.channelIdsLiteral(options.agentId) },
      },
    });
  }
}

module.exports = {
  SeriesData,
};

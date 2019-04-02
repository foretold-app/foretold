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
    return await this.models.Series.create(data);
  }

  /**
   * @param {object} options
   * @param {string} [options.agentId]
   * @return {Promise<*>}
   */
  async getAll(options) {
    const { channelId } = options;

    let where = {
        channelId: {
          [this.models.sequelize.Op.in]: this.channelIdsLiteral(options.agentId)
        },
      };

    if (channelId) {
      where.channelId = { [this.models.sequelize.Op.eq]: channelId };
    }

    return await this.models.Series.findAll({
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
    return await this.models.Series.findOne({
      where: {
        id,
        channelId: {
          [this.models.sequelize.Op.in]: this.channelIdsLiteral(options.agentId)
        },
      },
    });
  }
}

module.exports = {
  SeriesData,
};

const models = require("../models");

class SeriesData {
  /**
   * @param root
   * @param values
   * @param options
   * @return {Promise<*>}
   */
  async createSeries(root, values, options) {
    const newSeries = await models.Series.create({
      ...values,
      creatorId: options.user.agentId,
    });
    return newSeries;
  }

  /**
   * @param {string} id
   * @return {Promise<*>}
   */
  async getOne(id) {
    return await models.Series.findOne({ where: { id } });
  }
}

module.exports = {
  SeriesData,
};

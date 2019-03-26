const models = require("../models");

class SeriesData {

  /**
   * @tested
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
   * @tested
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

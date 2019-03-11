const models = require("../models");
const { notify } = require("../lib/notifications");


class SeriesData {
  /**
   * @todo: rename
   * @param root
   * @param values
   * @param options
   * @return {Promise<*>}
   */
  async createSeries(root, values, options) {
    const {
          name,
          description,
          channel,
          subjects,
          properties,
          dates,
    } = values;
    const _auth0Id = await this.usersData.getAuth0Id(options);
    const user = await this.usersData.auth0User(_auth0Id);
    const newSeries = await models.Sereies.create({
          name,
          description,
          channel,
          subjects,
          properties,
          dates,
          creatorId: user.agentId,
    });
    return newSeries;
  }

}

module.exports = {
  SeriesData,
};

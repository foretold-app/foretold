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
    // const _auth0Id = await this.usersData.getAuth0Id(options);
    // const user = await this.usersData.auth0User(_auth0Id);
    const newSeries = await models.Series.create({
          name,
          description,
          channel,
          subjects,
          properties,
          dates,
          creatorId: "51be4b31-b372-400e-8e58-c1f164ed9c63",
    });
    return newSeries;
  }

}

module.exports = {
  SeriesData,
};

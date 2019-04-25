const { API } = require('../api');

const config = require('../config');

class AggregationBot {
  constructor() {
    this.api = new API(config.BOT_TOKEN);
  }

  async main() {
    const measurables = await this.api.measurables();

    for(const measurable of measurables) {
      await this.api.measurementCreate({
        measurableId: measurable.id,
        floatPoint: 10.111,
      });
    }

    return true;
  }
}

module.exports = {
  AggregationBot,
};

const { API } = require('../api');

const config = require('../config');

class AggregationBot {
  constructor() {
    this.api = new API(config.BOT_TOKEN);
  }

  async main() {
    // await this.api.measurables();
    await this.api.measurementCreate();
    return true;
  }
}

module.exports = {
  AggregationBot,
};

const _ = require('lodash');
const { ProducerFeedItems } = require('./producer-feed-items');

class NewMeasurement extends ProducerFeedItems {

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _preload() {
    await super._preload();
    const channel = await this.input.getChannel();
    this.channelId = _.get(channel, 'id');
    return true;
  }
}

module.exports = {
  NewMeasurement,
};

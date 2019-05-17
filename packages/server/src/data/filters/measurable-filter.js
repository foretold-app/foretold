const _ = require('lodash');

class MeasurableFilter {
  constructor({ options }) {
    this.creatorId = _.get(options, 'creatorId');
    this.seriesId = _.get(options, 'seriesId');
    this.channelId = _.get(options, 'channelId');
    this.states = _.get(options, 'states');
    this.isArchived = _.get(options, 'isArchived');
  }
}

module.exports = {
  MeasurableFilter,
};

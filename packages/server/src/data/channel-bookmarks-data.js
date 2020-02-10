const { DataBase } = require('./data-base');
const { ChannelBookmarkModel } = require('../models');

const logger = require('../lib/log');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {VoteModel} model
 */
class ChannelBookmarksData extends DataBase {
  constructor() {
    super();
    this.model = new ChannelBookmarkModel();
    this.log = logger.module('ChannelBookmarksData');
  }
}

module.exports = {
  ChannelBookmarksData,
};

const { DataBase } = require('./data-base');
const { BookmarkModel } = require('../models');

const logger = require('../lib/log');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {VoteModel} model
 */
class BookmarksData extends DataBase {
  constructor() {
    super();
    this.model = new BookmarkModel();
    this.log = logger.module('BookmarksData');
  }
}

module.exports = {
  BookmarksData,
};

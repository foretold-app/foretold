const { DataBase } = require('./data-base');
const { VoteModel } = require('../models');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {MutexModel} model
 */
class VotesData extends DataBase {
  constructor() {
    super();
    this.model = new VoteModel();
  }
}

module.exports = {
  VotesData,
};

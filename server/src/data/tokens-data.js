const { DataBase } = require('./data-base');

const { TokenModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {AgentModel} AgentModel
 */
class TokensData extends DataBase {

  constructor() {
    super();
    this.model = new TokenModel();
  }

}

module.exports = {
  TokensData,
};

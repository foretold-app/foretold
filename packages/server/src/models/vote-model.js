const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModels.Generic}
 */
class VoteModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Vote,
      sequelize: models.sequelize,
    });
  }

  /**
   * @param _options
   * @returns {*}
   * @protected
   */
  _getAttributes(_options = {}) {
    return [
      [this.fn('SUM', this.col('voteAmount')), 'totalVoteAmount'],
    ];
  }

  /**
   * @param {Layers.AbstractModels.Options} _options
   * @protected
   */
  _getGroups(_options = {}) {
    return ['measurementId'];
  }
}

module.exports = {
  VoteModel,
};

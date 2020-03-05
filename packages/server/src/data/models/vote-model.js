const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.Models.ModelGeneric}
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
   * @param {Layers.Models.ModelOptions} _options
   * @protected
   */
  _getGroups(_options = {}) {
    return ['measurementId'];
  }
}

module.exports = {
  VoteModel,
};

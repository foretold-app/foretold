const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class VoteModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Vote,
      sequelize: models.sequelize,
    });
  }

  /**
   * @param {Layers.AbstractModelsLayer.options} _options
   * @return {{include: Sequelize.literal|*[]}}
   * @protected
   */
  _getAttributes(_options = {}) {
    return [
      [this.fn('SUM', this.col('voteAmount')), 'totalVoteAmount'],
    ];
  }

  /**
   * @param {Layers.AbstractModelsLayer.options} _options
   * @protected
   */
  _getGroups(_options = {}) {
    return ['measurementId'];
  }
}

module.exports = {
  VoteModel,
};

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
   * @return {{include: Sequelize.literal|*[]}}
   * @protected
   */
  _getAttributes() {
    return [
      [this.fn('SUM', this.col('voteAmount')), 'totalVoteAmount'],
    ];
  }

  /**
   * @protected
   */
  _getGroups() {
    return ['measurementId'];
  }
}

module.exports = {
  VoteModel,
};

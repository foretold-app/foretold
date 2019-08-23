const _ = require('lodash');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class AgentModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Agent,
      sequelize: models.sequelize,
    });
  }

  /**
   * @todo: see this._publicAndJoinedChannels()
   * @protected
   * @param {Models.ObjectID} channelId
   * @return {Sequelize.literal}
   */
  _agentsIdsLiteral(channelId) {
    return this.literal(this._agentsIds(channelId));
  }

  /**
   * @todo: Use ORM opportunities to join tables.
   * @protected
   * @param {Models.ObjectID} channelId
   * @return {string}
   */
  _agentsIds(channelId) {
    return `(
      SELECT "ChannelMemberships"."agentId" FROM "ChannelMemberships"
      WHERE "ChannelMemberships"."channelId" = '${channelId}'
    )`;
  }
}

module.exports = {
  AgentModel,
};

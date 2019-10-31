const _ = require('lodash');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class ChannelModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Channel,
      sequelize: models.sequelize,
    });
  }

  /**
   * @public
   * @todo: To decline in use of "this.models".
   * @param {Models.ChannelID} channelId
   * @return {Promise<Model[]>}
   */
  async getAgentsByChannelId(channelId) {
    const channel = await this.model.findOne({
      where: { id: channelId },
      include: [{
        model: this.models.Agent,
        as: 'agents',
      }],
    });
    return _.get(channel, 'agents', []);
  }
}

module.exports = {
  ChannelModel,
};

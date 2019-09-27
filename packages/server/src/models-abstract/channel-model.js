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
   * @param {Models.ObjectID} id
   * @return {Promise<Model[]>}
   */
  async getAgentsByChannelId(id) {
    const channel = await this.model.findOne({
      where: { id },
      include: [{
        model: this.models.Agent,
        as: 'agents'
      }],
    });
    return _.get(channel, 'agents', []);
  }
}

module.exports = {
  ChannelModel,
};

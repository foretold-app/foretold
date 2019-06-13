const _ = require('lodash');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

const { CHANNEL_MEMBERSHIP_ROLES } = require('../models/channel-membership-roles');

class ChannelMembershipModel extends ModelPostgres {
  constructor() {
    super({
      model: models.ChannelMemberships,
      sequelize: models.sequelize,
    });
  }

  /**
   * @todo: fix interface
   * @param {Models.ObjectID} channelId
   * @param {Models.ObjectID} agentId
   * @param {string} role
   * @returns {Promise<Models.ChannelMemberships>}
   */
  async updateOne(channelId, agentId, role) {
    const channelMembership = await this.model.findOne({
      where: { channelId, agentId },
    });
    if (channelMembership) {
      await channelMembership.update({ role });
    }
    return channelMembership;
  }

  /**
   * @todo: fix interface
   * @param {Models.ObjectID} channelId
   * @param {Models.ObjectID} agentId
   * @returns {Promise<Models.ChannelMemberships | null>}
   */
  async deleteOne(channelId, agentId) {
    const channelMembership = await this.model.findOne({
      where: { channelId, agentId },
    });
    if (channelMembership) {
      await this.model.destroy({
        where: { channelId, agentId },
      });
    }
    return channelMembership;
  }

}

ChannelMembershipModel.ROLES = CHANNEL_MEMBERSHIP_ROLES;

module.exports = {
  ChannelMembershipModel,
};

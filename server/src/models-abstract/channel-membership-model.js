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
   * @param {string} channelId
   * @param {string} agentId
   * @param {string} role
   * @returns {Promise<Models.ChannelMemberships>}
   */
  async updateOne(channelId, agentId, role) {
    const transaction = await this.getTransaction();
    try {
      const channelMembership = await this.model.findOne({
        where: { channelId, agentId },
        transaction
      });
      if (channelMembership) {
        await channelMembership.update({
          role,
        }, {
          transaction
        });
        await this.checkCountOfAdmins(channelId, transaction);
        await transaction.commit();
      }
      return channelMembership;
    } catch (e) {
      await transaction.rollback();
      return Promise.reject(e);
    }
  }

  /**
   * @param {string} channelId
   * @param {string} agentId
   * @returns {Promise<Models.ChannelMemberships | null>}
   */
  async deleteOne(channelId, agentId) {
    const transaction = await this.getTransaction();
    try {
      const channelMembership = await this.model.findOne({
        where: { channelId, agentId },
        transaction
      });
      if (channelMembership) {
        await this.model.destroy({
          where: { channelId, agentId },
          transaction,
        });
        await this.checkCountOfAdmins(channelId, transaction);
        await transaction.commit();
      }
      return channelMembership;
    } catch (e) {
      await transaction.rollback();
      return Promise.reject(e);
    }
  }

  /**
   * @param {string} channelId
   * @param {object} transaction
   * @return {Promise<*>}
   */
  async checkCountOfAdmins(channelId, transaction) {
    const countOfAdmins = await this.model.count({
      where: {
        channelId,
        role: ChannelMembershipModel.ROLES.ADMIN,
      },
      transaction,
    });
    if (countOfAdmins === 0) {
      // @todo: move into permissions
      throw new Error('Operation is not permitted.');
    }
    return countOfAdmins;
  }
}

ChannelMembershipModel.ROLES = CHANNEL_MEMBERSHIP_ROLES;

module.exports = {
  ChannelMembershipModel,
};

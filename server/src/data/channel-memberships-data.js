const _ = require('lodash');

const models = require("../models");

const { DataBase } = require('./data-base');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 */
class ChannelMembershipsData extends DataBase {

  /**
   * @param {string} channelId
   * @param {string} agentId
   * @param {string} role
   * @returns {Promise<Models.ChannelMemberships>}
   */
  async createOne(channelId, agentId, role) {
    await this.validate({ channelId, agentId });
    const channelMembership =
      await models.ChannelMemberships.findOne({ where: { channelId, agentId } }) ||
      await models.ChannelMemberships.create({ channelId, agentId, role });
    return channelMembership;
  }

  /**
   * @param {string} channelId
   * @param {string} agentId
   * @param {string} role
   * @returns {Promise<Models.ChannelMemberships>}
   */
  async updateOne(channelId, agentId, role) {
    await this.validate({ channelId, agentId });
    const channelMembership = await models.ChannelMemberships.findOne({
      where: { channelId, agentId }
    });
    if (channelMembership) {
      await channelMembership.update({ role });
    }
    return channelMembership;
  }

  /**
   * @param {string} channelId
   * @param {string} agentId
   * @returns {Promise<Models.ChannelMemberships | null>}
   */
  async deleteOne(channelId, agentId) {
    const input = { channelId, agentId };
    await this.validate(input);
    const channelMembership = await models.ChannelMemberships.findOne({ where: input });
    if (channelMembership) {
      await models.ChannelMemberships.destroy({ where: input });
    }
    return channelMembership;
  }

  /**
   * @param {string} channelId
   * @param {string} agentId
   * @return {Promise<*>}
   */
  async validate({ channelId, agentId }) {
    if (!await models.Channel.findById(channelId)) {
      return Promise.reject(new Error(`Channel "${ channelId }" is not found.`));
    }
    if (!await models.Agent.findById(agentId)) {
      return Promise.reject(new Error(`Agent "${ agentId }" is not found.`));
    }
    return true;
  }

  /**
   *
   * @todo: Call "getOne()" of "model" layer.
   * @todo: Pass "id", "restrictions" and "filter" params
   * @todo: right into model call.
   * @todo: Then do the same everywhere on data layer.
   *
   * @param {object} options
   * @param {string} options.agentId?
   * @param {string} options.channelId?
   * @returns {Promise<Models.ChannelMemberships>}
   */
  async getOne(options) {
    return await models.ChannelMemberships.findOne({ where: options });
  }

  /**
   * @param {object} options
   * @param {string} options.agentId?
   * @param {string} options.channelId?
   * @returns {Promise<Models.ChannelMemberships[]>}
   */
  async getAll(options) {
    return await models.ChannelMemberships.findAll({ where: options });
  }

  /**
   * @param {object} options
   * @param {string} [options.agentId]
   * @param {string} [options.channelId]
   * @returns {Promise<string[]>}
   */
  async getAllChannelIds(options) {
    return (await this.getAll(options)).map(o => o.channelId);
  }

}

module.exports = {
  ChannelMembershipsData,
};

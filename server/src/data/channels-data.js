const _ = require('lodash');

const models = require("../models");

const { UsersData } = require('./users-data');

class ChannelsData {

  constructor() {
    this.usersData = new UsersData();
  }

  async channelUpdate(root, values, options) {
    console.log(root, values, options);
    return true;
  }

  async channelCreate(root, values, options) {
    // @todo: fix
    const auth0Id = await this.usersData.getAuth0Id(options);
    const user = await this.usersData.auth0User(auth0Id);

    const input = _.get(values, 'input');
    const channel = await this.createOne(user, input);

    return channel;
  }

  async addPersonToChannel(root, values, options) {
    console.log(root, values, options);
    return true;
  }

  async removePersonFromChannel(root, values, options) {
    console.log(root, values, options);
    return true;
  }

  /**
   * @param {Model} user
   * @param {object} input
   * @return {Promise<Model>}
   */
  async createOne(user, input) {
    return await models.Channel.findOne({
      where: { name: input.name },
    }) || await models.Channel.create({
      ...input,
      creatorId: user.agentId,
      agents: [{ id: user.agent_id }]
    });
  }

}

module.exports = {
  ChannelsData,
};

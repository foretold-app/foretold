const _ = require('lodash');
const assert = require('assert');

const { DataBase } = require('./data-base');

const { InvitationModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {InvitationModel} InvitationModel
 */
class InvitationsData extends DataBase {
  constructor() {
    super();
    this.model = new InvitationModel();
  }

  /**
   * @public
   * @todo: Use transactions here.
   * @param {object} input
   * @param {string} input.email
   * @param {string} input.channelId
   * @param {string} input.inviterAgentId
   * @return {Promise<Object>}
   */
  async invite(input) {
    try {
      const channelId = _.get(input, 'channelId');
      const email = _.get(input, 'email');
      const inviterAgentId = _.get(input, 'inviterAgentId');

      assert(_.isString(email), 'Email should be a string');
      assert(_.isString(channelId), 'Channel Id should be a string');
      assert(_.isString(inviterAgentId), 'Inviter Agent Id should be a string');

      const params = { email, channelId };
      const query = {};
      const data = { email, inviterAgentId, channelId };
      return await this.upsertOne(params, query, data);

    } catch (e) {
      console.error('Invitation Err', e);
    }
    return null;
  }
}

module.exports = {
  InvitationsData,
};

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
    this.InvitationModel = new InvitationModel();
    this.model = this.InvitationModel;
  }

  /**
   * @public
   * @param {object} datas
   * @param {string} datas.email
   * @param {string} datas.channelId
   * @param {string} datas.inviterAgentId
   * @return {Promise<*>}
   */
  async invite(datas) {
    assert(_.isString(datas.email), 'Email should be an string');
    assert(_.isString(datas.channelId), 'Channel Id should be an string');
    assert(_.isString(datas.inviterAgentId), 'Inviter Agent Id should be an string');
  }

}

module.exports = {
  InvitationsData,
};

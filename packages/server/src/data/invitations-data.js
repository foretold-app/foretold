const _ = require('lodash');

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

}

module.exports = {
  InvitationsData,
};

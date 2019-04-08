const _ = require('lodash');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

class ChannelMembershipModel extends ModelPostgres {
  constructor() {
    super({
      model: models.ChannelMemberships,
      sequelize: models.sequelize,
    });
  }
}

ChannelMembershipModel.ROLES = models.ChannelMemberships.ROLES;

module.exports = {
  ChannelMembershipModel,
};

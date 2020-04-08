const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

const { CHANNEL_MEMBERSHIP_ROLES } = require('../../enums');

class ChannelMembershipModel extends ModelPostgres {
  constructor() {
    super({
      model: models.ChannelMemberships,
      sequelize: models.sequelize,
    });
  }
}

ChannelMembershipModel.ROLES = CHANNEL_MEMBERSHIP_ROLES;

module.exports = {
  ChannelMembershipModel,
};

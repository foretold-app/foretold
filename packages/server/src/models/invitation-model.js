const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.Models.Generic}
 */
class InvitationModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Invitation,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  InvitationModel,
};

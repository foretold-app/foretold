const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
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

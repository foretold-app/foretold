const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class TemplateModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Template,
      sequelize: models.sequelize,
    });
  }

}

module.exports = {
  TemplateModel,
};

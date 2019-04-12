const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

class BotModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Bot,
      sequelize: models.sequelize,
    });
  }

  createOne(data) {
    return this.model.create(data);
  }
}

module.exports = {
  BotModel,
};

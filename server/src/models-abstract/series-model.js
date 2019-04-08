const _ = require('lodash');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

class SeriesModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Series,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  SeriesModel,
};

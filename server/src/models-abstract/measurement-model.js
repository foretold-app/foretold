const _ = require('lodash');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

class MeasurementModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Measurement,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  MeasurementModel,
};

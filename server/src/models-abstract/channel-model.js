const _ = require('lodash');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

class ChannelModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Channel,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  ChannelModel,
};

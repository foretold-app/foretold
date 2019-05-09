const models = require('../models');

class DataBase {

  /**
   * @todo: remove this property then
   */
  constructor() {
    this.models = models;
  }

}

module.exports = {
  DataBase,
};



const { DataBase } = require('./data-base');
const { TemplateModel } = require('../models');

/**
 * @implements {Layers.DataSource.Generic}
 * @property {TemplateModel} model
 */
class TemplatesData extends DataBase {
  constructor() {
    super();
    this.model = new TemplateModel();
  }
}

module.exports = {
  TemplatesData,
};

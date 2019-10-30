const { DataBase } = require('./data-base');

const { TemplateModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
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

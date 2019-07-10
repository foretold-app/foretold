const { DataBase } = require('./data-base');

const { TemplateModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {TemplateModel} TemplatesModel
 */
class TemplatesData extends DataBase {

  constructor() {
    super();
    this.TemplateModel = new TemplateModel();
    this.model = this.TemplateModel;
  }

}

module.exports = {
  TemplatesData,
};

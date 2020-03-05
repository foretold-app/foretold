const { DataBase } = require('./data-base');
const { TemplateModel } = require('./models');

/**
 * @implements {Layers.DataSource.DataGeneric}
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

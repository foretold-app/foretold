const { DataBase } = require('./data-base');
const { NotebookModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {NotebookModel} model
 */
class NotebooksData extends DataBase {
  constructor() {
    super();
    this.model = new NotebookModel();
  }
}

module.exports = {
  NotebooksData,
};

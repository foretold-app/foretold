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

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getDefaultRestrictions(options = {}) {
    return {
      ...super._getDefaultRestrictionsForIncludedIntoChannel(options),
    };
  }
}

module.exports = {
  NotebooksData,
};

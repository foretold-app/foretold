const { DataBase } = require('./data-base');
const { NotebookModel } = require('../models');

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
   * @param {Layers.DataSourceLayer.Options} [options]
   * @return {Layers.AbstractModelsLayer.Restrictions}
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

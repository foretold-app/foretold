const { DataBase } = require('./data-base');
const { NotebookModel } = require('../models');

/**
 * @implements {Layers.DataSource.DataGeneric}
 * @property {NotebookModel} model
 */
class NotebooksData extends DataBase {
  constructor() {
    super();
    this.model = new NotebookModel();
  }

  /**
   * @protected
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {Layers.Models.ModelRestrictions}
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

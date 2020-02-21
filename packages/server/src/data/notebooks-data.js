const { DataBase } = require('./data-base');
const { NotebookModel } = require('../models');

/**
 * @implements {Layers.DataSource.Generic}
 * @property {NotebookModel} model
 */
class NotebooksData extends DataBase {
  constructor() {
    super();
    this.model = new NotebookModel();
  }

  /**
   * @protected
   * @param {Layers.DataSource.Options} [options]
   * @return {Layers.Models.Restrictions}
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

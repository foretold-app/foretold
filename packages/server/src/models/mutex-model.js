const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.Models.ModelGeneric}
 */
class MutexModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Mutex,
      sequelize: models.sequelize,
    });
  }

  /**
   * @param {Layers.Models.ModelOptions} options
   * @returns {Promise<*>}
   */
  async lock(options) {
    return this._lockTable('Mutexes', options);
  }
}

module.exports = {
  MutexModel,
};

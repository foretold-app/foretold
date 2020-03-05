const _ = require('lodash');

const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');
const templates = require('../../../config/templates');

/**
 * @implements {Layers.Models.ModelGeneric}
 */
class TemplateModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Template,
      sequelize: models.sequelize,
    });
  }


  /**
   * It is a temporary solution.
   * @public
   * @param {object} [params]
   * @param {string} params.name
   * @param {object} [_query]
   * @param {Layers.Models.ModelRestrictions} [_restrictions]
   * @param {Layers.Models.ModelOptions} [_options]
   * @return {Promise<Models.Definition>}
   */
  async getOne(params = {}, _query = {}, _restrictions = {}, _options = {}) {
    if (!params.name) return null;
    const template = _.find(templates, ['name', params.name]);
    return new this.model(template);
  }
}

module.exports = {
  TemplateModel,
};

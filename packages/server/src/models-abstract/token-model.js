const _ = require('lodash');

const models = require('../models');
const { TOKEN_TYPE } = require('../models/enums/token-type');

const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class TokenModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Token,
      sequelize: models.sequelize,
    });
  }

  /**
   * @param {Models.ObjectID} [agentId]
   * @param {string} [token]
   * @param {string} [type]
   * @param {number} [usageCount]
   * @return {Promise<Models.Token>}
   */
  async getToken({ agentId, token, type, usageCount }) {
    type = type || TOKEN_TYPE.ACCESS_TOKEN;
    usageCount = usageCount || 3;

    const cond = {
      [this.and]: [
        { type, isActive: true, },
        {
          [this.or]: [
            { expiresAt: null },
            { expiresAt: { [this.gt]: this.fn('now') } },
          ],
        },
        {
          [this.or]: [
            { usageCount: null },
            { usageCount: { [this.lt]: usageCount } },
          ],
        },
      ],
    };

    if (agentId) cond.agentId = agentId;
    if (token) cond.token = token;

    const options = { sort: -1 };
    return this.model.findOne({ where: cond }, options);
  }
}

module.exports = {
  TokenModel,
};

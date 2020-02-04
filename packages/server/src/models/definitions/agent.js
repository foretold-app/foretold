const _ = require('lodash');
const Sequelize = require('sequelize');

const { AGENT_TYPE } = require('../../enums');

module.exports = (sequelize, DataTypes) => {
  const Agent = sequelize.define('Agent', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: DataTypes.UUIDV4,
      allowNull: false,
    },
    type: {
      type: DataTypes.ENUM([
        AGENT_TYPE.USER,
        AGENT_TYPE.BOT,
      ]),
      allowNull: false,
    },
    isAdmin: {
      type: DataTypes.BOOLEAN,
      allowNull: false,
      defaultValue: false,
    },
    name: {
      allowNull: true,
      type: Sequelize.VIRTUAL(DataTypes.STRING),
      get: getName,
    },
    createdAt: {
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
      allowNull: false,
    },
    updatedAt: {
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
      allowNull: false,
    },
  });

  /**
   * @todo: Are we already ready to remove this?
   * @deprecated: Hardly deprecated!
   * @returns {Promise<undefined>}
   */
  async function getName() {
    if (this.type === AGENT_TYPE.USER) {
      const user = await this.getUser();
      return _.get(user, 'name');
    }
    const bot = await this.getBot();
    return _.get(bot, 'name');
  }

  /**
   * @todo: To fix it, remove it!
   * @deprecated
   * @param models
   */
  Agent.associate = function associate(models) {
    Agent.User = Agent.hasOne(models.User, {
      foreignKey: 'agentId',
    });

    Agent.Bot = Agent.hasOne(models.Bot, {
      foreignKey: 'agentId',
    });
  };

  return Agent;
};

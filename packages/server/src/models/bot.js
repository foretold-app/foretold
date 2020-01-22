const { MEASUREMENT_COMPETITOR_TYPE } = require('../enums');

module.exports = (sequelize, DataTypes) => {
  const Bot = sequelize.define('Bot', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: DataTypes.UUIDV4,
      allowNull: false,
    },
    name: {
      type: DataTypes.STRING,
      allowNull: true,
    },
    description: {
      type: DataTypes.STRING,
      allowNull: true,
    },
    picture: {
      type: DataTypes.STRING(512),
      allowNull: true,
    },
    competitorType: {
      type: DataTypes.ENUM([
        MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE,
        MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE,
        MEASUREMENT_COMPETITOR_TYPE.AGGREGATION,
      ]),
      defaultValue: MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE,
      allowNull: true,
    },
    userId: {
      type: DataTypes.UUID(),
    },
    agentId: {
      type: DataTypes.UUID(),
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
   * @todo: fix it, remove it.
   * @deprecated
   * @param models
   */
  Bot.associate = function associate(models) {
    Bot.User = Bot.belongsTo(models.User, {
      foreignKey: 'userId',
    });

    Bot.Agent = Bot.belongsTo(models.Agent, {
      foreignKey: 'agentId',
    });
  };

  return Bot;
};

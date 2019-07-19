const { MEASUREMENT_COMPETITOR_TYPE } = require('./enums/measurement-competitor-type');
const { AGENT_TYPE } = require('./enums/agent-type');

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
      defaultValue: DataTypes.NOW,
    },
    updatedAt: {
      type: DataTypes.DATE,
      defaultValue: DataTypes.NOW,
    },
  });

  Bot.addHook('beforeCreate', async (event) => {
    let agent = await sequelize.models.Agent.create({
      type: AGENT_TYPE.BOT,
    });
    event.agentId = agent.dataValues.id;
  });

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

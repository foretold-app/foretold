const { MEASUREMENT_COMPETITOR_TYPE } = require('./enums/measurement-competitor-type');
const { AGENT_TYPE } = require('./enums/agent-type');

module.exports = (sequelize, DataTypes) => {
  const Model = sequelize.define('Bot', {
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
  });

  Model.addHook('beforeCreate', async (event) => {
    let agent = await sequelize.models.Agent.create({
      type: AGENT_TYPE.BOT,
    });
    event.agentId = agent.dataValues.id;
  });

  Model.associate = function associate(models) {
    Model.User = Model.belongsTo(models.User, {
      foreignKey: 'userId',
    });

    Model.Agent = Model.belongsTo(models.Agent, {
      foreignKey: 'agentId',
    });
  };

  return Model;
};

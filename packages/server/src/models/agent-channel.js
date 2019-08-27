module.exports = (sequelize, DataTypes) => {
  const AgentChannel = sequelize.define('AgentChannel', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: sequelize.fn('uuid_generate_v4'),
      allowNull: false,
    },
    agentId: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    channelId: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    primaryPointScore: {
      type: DataTypes.FLOAT,
      allowNull: false,
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

  AgentChannel.associate = function associate(models) {
    AgentChannel.Agent = AgentChannel.belongsTo(models.Agent, {
      foreignKey: 'agentId',
    });
    AgentChannel.Channel = AgentChannel.belongsTo(models.Channel, {
      foreignKey: 'channelId',
    });
  };

  return AgentChannel;
};

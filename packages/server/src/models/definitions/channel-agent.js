module.exports = (sequelize, DataTypes) => {
  const ChannelAgent = sequelize.define('ChannelAgent', {
    agentId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    channelId: {
      type: DataTypes.UUID(),
      allowNull: false,
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

  ChannelAgent.removeAttribute('id');

  return ChannelAgent;
};

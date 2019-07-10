module.exports = (sequelize, DataTypes) => {

  const AgentNotification = sequelize.define('AgentNotification', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: DataTypes.UUIDV4,
      allowNull: false,
    },
    agentId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    notificationId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    sentAt: {
      type: DataTypes.DATE,
      allowNull: true,
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

  AgentNotification.associate = function associate(models) {

    AgentNotification.Agent =
      AgentNotification.belongsTo(models.Agent, {
        foreignKey: 'agentId',
        as: 'agent',
      });

    AgentNotification.Notification =
      AgentNotification.belongsTo(models.Notification, {
        foreignKey: 'notificationId',
        as: 'notification',
      });

  };

  return AgentNotification;
};

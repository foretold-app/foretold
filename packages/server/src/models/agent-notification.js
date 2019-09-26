const {
  NOTIFICATION_ERROR_REASON,
} = require('../enums/notification-error-reason');

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
    attemptCounter: {
      type: DataTypes.INTEGER,
      allowNull: false,
      defaultValue: 0,
    },
    errorReason: {
      type: DataTypes.ENUM([
        NOTIFICATION_ERROR_REASON.NONE,
        NOTIFICATION_ERROR_REASON.INTERNAL_ERROR,
        NOTIFICATION_ERROR_REASON.EXTERNAL_ERROR,
        NOTIFICATION_ERROR_REASON.PREFERENCES_ERROR,
        NOTIFICATION_ERROR_REASON.EMAIL_ADDRESS_ERROR,
      ]),
      defaultValue: NOTIFICATION_ERROR_REASON.NONE,
    },
    errorAt: {
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
    AgentNotification.Agent = AgentNotification.belongsTo(models.Agent, {
      foreignKey: 'agentId',
      as: 'agent',
    });

    AgentNotification.Notification = AgentNotification.belongsTo(
      models.Notification, {
        foreignKey: 'notificationId',
        as: 'notification',
      });
  };

  return AgentNotification;
};

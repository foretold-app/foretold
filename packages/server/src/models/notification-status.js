const { NOTIFICATION_ERROR_REASON } = require('../enums');

module.exports = (sequelize, DataTypes) => {
  const NotificationStatus = sequelize.define('NotificationStatus', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: DataTypes.UUIDV4,
      allowNull: false,
    },
    agentId: {
      type: DataTypes.UUID(),
      allowNull: true,
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
      defaultValue: sequelize.fn('statement_timestamp'),
    },
    updatedAt: {
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
    },
  });

  NotificationStatus.associate = function associate(models) {
    NotificationStatus.Agent = NotificationStatus.belongsTo(models.Agent, {
      foreignKey: 'agentId',
      as: 'agent',
    });

    NotificationStatus.Notification = NotificationStatus.belongsTo(
      models.Notification, {
        foreignKey: 'notificationId',
        as: 'notification',
      },
    );
  };

  return NotificationStatus;
};

const { NOTIFICATION_TYPE } = require('./enums/notification-type');

module.exports = (sequelize, DataTypes) => {

  const Notification = sequelize.define('Notification', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: DataTypes.UUIDV4,
      allowNull: false,
    },
    type: {
      type: DataTypes.ENUM([
        NOTIFICATION_TYPE.EMAIL,
        NOTIFICATION_TYPE.PUSH,
        NOTIFICATION_TYPE.WEB_PUSH,
      ]),
      defaultValue: NOTIFICATION_TYPE.EMAIL,
    },
    envelope: {
      type: DataTypes.JSON,
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

  Notification.associate = function associate(_models) {
    //
  };

  return Notification;
};

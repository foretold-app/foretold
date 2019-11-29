const { NOTIFICATION_TYPE } = require('../enums');

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
      defaultValue: sequelize.fn('statement_timestamp'),
    },
    updatedAt: {
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
    },
  });

  return Notification;
};

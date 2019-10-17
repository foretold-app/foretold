const { MUTEX_STATUS } = require('../enums/mutex-status');

module.exports = (sequelize, DataTypes) => {
  const Mutex = sequelize.define('Mutex', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: DataTypes.UUIDV4,
      allowNull: false,
    },
    name: {
      type: DataTypes.STRING(512),
      allowNull: false,
      unique: true,
    },
    status: {
      type: DataTypes.ENUM([
        MUTEX_STATUS.FREE,
        MUTEX_STATUS.CAPTURED,
      ]),
      allowNull: false,
    },
    agentId: {
      type: DataTypes.UUID(),
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

  Mutex.associate = function associate(models) {
    Mutex.Creator = Mutex.belongsTo(models.Agent, {
      foreignKey: 'agentId',
      as: 'agent',
    });
  };

  return Mutex;
};

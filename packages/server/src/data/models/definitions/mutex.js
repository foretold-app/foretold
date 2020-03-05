module.exports = (sequelize, DataTypes) => {
  const Mutex = sequelize.define('Mutex', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: DataTypes.UUIDV4,
      allowNull: false,
    },
    name: {
      type: DataTypes.STRING(128),
      allowNull: false,
    },
    agentId: {
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
  return Mutex;
};

module.exports = (sequelize, DataTypes) => {
  const GlobalSetting = sequelize.define('GlobalSetting', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: sequelize.fn('uuid_generate_v4'),
      allowNull: false,
    },
    // Always use this field
    // since UUID4 is not ordered.
    name: {
      type: DataTypes.STRING,
      allowNull: false,
      unique: true,
    },
    entityGraph: {
      type: DataTypes.JSON,
      allowNull: true,
    },
    botAgentId: {
      type: DataTypes.UUID(),
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

  return GlobalSetting;
};

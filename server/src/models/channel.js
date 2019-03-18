const Sequelize = require('sequelize');

module.exports = (sequelize, DataTypes) => {
  const Model = sequelize.define('Channel', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: Sequelize.UUIDV4,
      allowNull: false,
    },
    name: {
      type: DataTypes.STRING,
      allowNull: false,
      unique: true,
    },
    description: {
      type: DataTypes.TEXT,
      allowNull: true,
    },
    creatorId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    isArchived: {
      type: DataTypes.BOOLEAN,
      allowNull: false,
      default: false,
    },
    isPublic: {
      type: DataTypes.BOOLEAN,
      allowNull: false,
      default: false,
    },
  });

  return Model;
};

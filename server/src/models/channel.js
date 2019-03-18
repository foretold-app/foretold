/**
 * No migrations needed.
 * @param sequelize
 * @param DataTypes
 * @return {*}
 */
module.exports = (sequelize, DataTypes) => {
  const Channel = sequelize.define('Channel', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: sequelize.UUIDV4,
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
    // Explicit Link
    creatorId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    // Booleans
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


  Channel.associate = function (models) {
    Channel.Creator = Channel.belongsTo(models.Agent, {
      foreignKey: 'creatorId',
      as: 'creator'
    });
  };

  return Channel;
};

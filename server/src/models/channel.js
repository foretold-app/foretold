/**
 * @todo: Rename table to "channels".
 * @param sequelize
 * @param DataTypes
 * @return {*}
 */
module.exports = (sequelize, DataTypes) => {
  const Channel = sequelize.define('Channel', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: sequelize.fn('uuid_generate_v4'),
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
    isArchived: {
      type: DataTypes.BOOLEAN,
      allowNull: false,
      default: false,
    },
    isPublic: {
      type: DataTypes.BOOLEAN,
      allowNull: false,
      default: true,
    },
  });


  Channel.associate = function (models) {
    Channel.Creator = Channel.belongsTo(models.Agent, {
      foreignKey: 'creatorId',
      as: 'creator'
    });
    Channel.Agents = Channel.belongsToMany(models.Agent, {
      through: models.AgentsChannels,
      foreignKey: 'channelId',
      otherKey: 'agentId',
      as: 'agents',
    });
  };

  return Channel;
};

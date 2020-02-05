/**
 * @param sequelize
 * @param DataTypes
 * @return {*}
 */
module.exports = (sequelize, DataTypes) => {
  const Channel = sequelize.define('Channel', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: DataTypes.UUIDV4,
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
      defaultValue: false,
    },
    isPublic: {
      type: DataTypes.BOOLEAN,
      allowNull: false,
      defaultValue: true,
    },
    isCurated: {
      type: DataTypes.BOOLEAN,
      allowNull: false,
      defaultValue: false,
    },
    // @entity: creator-link
    creatorId: {
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

  /**
   * @todo: fix it, remove it!
   * @deprecated
   * @param models
   */
  Channel.associate = function associate(models) {
    // const ch = await models.Channel
    //   .findByPk('406da139-e440-4c74-bb3c-514ed1872cea');
    // const ag = await ch.getAgents();
    Channel.Agents = Channel.belongsToMany(models.Agent, {
      through: models.ChannelMemberships,
      foreignKey: 'channelId',
      otherKey: 'agentId',
      as: 'agents',
    });
  };

  return Channel;
};

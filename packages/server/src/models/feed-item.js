module.exports = (sequelize, DataTypes) => {
  const FeedItem = sequelize.define('FeedItem', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: sequelize.fn('uuid_generate_v4'),
      allowNull: false,
    },
    channelId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    agentId: {
      type: DataTypes.UUID(),
      allowNull: true,
    },
    body: {
      type: DataTypes.JSON,
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
   * @todo: fix it, remove it.
   * @deprecated
   * @param models
   */
  FeedItem.associate = function associate(models) {
    FeedItem.Channel = FeedItem.belongsTo(models.Channel, {
      foreignKey: 'channelId',
    });
    FeedItem.Agent = FeedItem.belongsTo(models.Agent, {
      foreignKey: 'agentId',
    });
  };

  return FeedItem;
};

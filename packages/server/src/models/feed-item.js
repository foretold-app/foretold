module.exports = (sequelize, DataTypes) => {

  const FeedItem = sequelize.define('FeedItem', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: sequelize.fn('uuid_generate_v4'),
      allowNull: false,
    },
    channelId: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    body: {
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

  FeedItem.associate = function associate(models) {
    FeedItem.Channel = FeedItem.belongsTo(models.Channel, {
      foreignKey: 'channelId',
    });
  };

  return FeedItem;
};

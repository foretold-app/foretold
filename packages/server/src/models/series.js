module.exports = (sequelize, DataTypes) => {
  const Series = sequelize.define('Series', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: DataTypes.UUIDV4,
      allowNull: false,
    },
    name: {
      type: DataTypes.STRING(255),
      allowNull: true,
    },
    description: {
      type: DataTypes.STRING,
      allowNull: true,
    },
    subjects: {
      type: DataTypes.ARRAY(DataTypes.STRING(255)),
      allowNull: true,
    },
    properties: {
      type: DataTypes.ARRAY(DataTypes.STRING(255)),
      allowNull: true,
    },
    dates: {
      type: DataTypes.ARRAY(DataTypes.DATE),
      allowNull: true,
    },
    channelId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    creatorId: {
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

  Series.associate = function associate(models) {
    Series.Creator = Series.belongsTo(models.Agent, {
      foreignKey: 'creatorId',
      as: 'creator',
    });

    Series.Measurables = Series.hasMany(models.Measurable, {
      foreignKey: 'seriesId',
      as: 'Measurables',
    });

    // Usage
    // const se = await models.Series.find();
    // const ch = await se.getChannel();
    Series.Channel = Series.belongsTo(models.Channel, {
      foreignKey: 'channelId',
    });
  };

  return Series;
};

module.exports = (sequelize, DataTypes) => {
  const Series = sequelize.define('Series', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: DataTypes.UUIDV4,
      allowNull: false,
    },
    name: {
      type: DataTypes.STRING,
      allowNull: true,
    },
    description: {
      type: DataTypes.STRING,
      allowNull: true,
    },
    subjects: {
      type: DataTypes.ARRAY(DataTypes.STRING),
      allowNull: true,
    },
    properties: {
      type: DataTypes.ARRAY(DataTypes.STRING),
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
    // @entity: creator-link
    creatorId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    createdAt: {
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
    },
    updatedAt: {
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
    },
  });

  /**
   * @todo: fix it, remove it.
   * @deprecated
   * @param models
   */
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

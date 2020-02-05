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
      allowNull: false,
    },
    updatedAt: {
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
      allowNull: false,
    },
  });

  return Series;
};

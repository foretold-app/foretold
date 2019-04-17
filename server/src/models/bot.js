module.exports = (sequelize, DataTypes) => {
  const Model = sequelize.define('Bot', {
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
    competitorType: {
      type: DataTypes.ENUM(["COMPETITIVE", "AGGREGATION", "OBJECTIVE"]),
      defaultValue: "COMPETITIVE",
      allowNull: true,
    },
  });

  Model.associate = function (models) {
    Model.User = Model.belongsTo(models.User, {
      foreignKey: 'ownerUserId',
    });

    Model.Agent = Model.belongsTo(models.Agent, {
      foreignKey: 'ownerAgentId',
    })
  };

  return Model;
};

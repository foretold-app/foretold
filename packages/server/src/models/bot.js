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
      userId: {
        type: DataTypes.UUID(),
        // allowNull: false,
      },
      agentId: {
        type: DataTypes.UUID(),
        // allowNull: false,
      },
    },
    {
      hooks: {
        beforeCreate: async (event) => {
          let agent = await sequelize.models.Agent.create({
            type: "BOT",
          });
          event.agentId = agent.dataValues.id;
        },
      }
    });

  Model.associate = function (models) {
    Model.User = Model.belongsTo(models.User, {
      foreignKey: 'userId',
    });

    Model.Agent = Model.belongsTo(models.Agent, {
      foreignKey: 'agentId',
    });
  };

  return Model;
};

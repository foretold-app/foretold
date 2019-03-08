const Sequelize = require('sequelize');

module.exports = (sequelize, DataTypes) => {
  var Model = sequelize.define('User', {
      id: {
        type: DataTypes.UUID(),
        primaryKey: true,
        defaultValue: Sequelize.UUIDV4,
        allowNull: false,
      },
      name: {
        type: DataTypes.STRING,
        allowNull: false,
      },
      auth0Id: {
        type: DataTypes.STRING,
        allowNull: true,
      }
    },
    {
      hooks: {
        beforeCreate: async (event, options) => {
          let agent = await sequelize.models.Agent.create({
            type: "USER",
          });
          event.agentId = agent.dataValues.id
        }
      }
    }
  );
  Model.associate = function (models) {
    Model.Agent = Model.belongsTo(models.Agent, { foreignKey: 'agentId' });
    Model.Bots = Model.hasMany(models.Bot, { foreignKey: 'userId' });
  };
  return Model;
};

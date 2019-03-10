const Sequelize = require('sequelize');

/**
 * @param {Sequelize} sequelize
 * @param {sequelize.DataTypes} DataTypes
 * @return {*|Model<any, any>|void|Model}
 */
module.exports = (sequelize, DataTypes) => {
  const Model = sequelize.define('User', {
      id: {
        type: DataTypes.STRING(32),
        primaryKey: true,
        defaultValue: sequelize.fn('generate_object_id'),
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

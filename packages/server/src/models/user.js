const { AGENT_TYPE } = require('./enums/agent-type');

module.exports = (sequelize, DataTypes) => {
  const Model = sequelize.define('User', {
      id: {
        type: DataTypes.UUID(),
        primaryKey: true,
        defaultValue: DataTypes.UUIDV4,
        allowNull: false,
      },
      name: {
        type: DataTypes.STRING,
        allowNull: false,
      },
      auth0Id: {
        type: DataTypes.STRING,
        allowNull: true,
      },
      nickname: {
        type: DataTypes.STRING(32),
        allowNull: true,
      },
      email: {
        type: DataTypes.STRING(64),
        allowNull: true,
        validate: {
          isEmail: true,
        }
      },
      picture: {
        type: DataTypes.STRING(128),
        allowNull: true,
      },
    },
    {
      hooks: {
        beforeCreate: async (event) => {
          let agent = await sequelize.models.Agent.create({
            type: AGENT_TYPE.USER,
          });
          event.agentId = agent.dataValues.id
        }
      }
    }
  );

  Model.associate = function (models) {
    Model.Agent = Model.belongsTo(models.Agent, {
      foreignKey: 'agentId',
    });

    Model.Bots = Model.hasMany(models.Bot, {
      foreignKey: 'userId',
    });
  };

  return Model;
};

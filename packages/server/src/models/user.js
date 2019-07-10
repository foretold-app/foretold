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
        type: DataTypes.STRING(30),
        allowNull: false,
        defaultValue: '',
        validate: {
          is: ['^[a-z0-9]{0,30}$', 'i'],
        }
      },
      auth0Id: {
        type: DataTypes.STRING,
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
        type: DataTypes.STRING(255),
        allowNull: true,
      },
      isEmailVerified: {
        type: DataTypes.BOOLEAN,
        allowNull: true,
      },
      agentId: {
        type: DataTypes.UUID(),
        allowNull: true,
      },
    },
  );

  Model.addHook('beforeCreate', async (event) => {
    const agent = await sequelize.models.Agent.create({
      type: AGENT_TYPE.USER,
    });
    event.agentId = agent.dataValues.id
  });

  Model.associate = function associate(models) {
    Model.Agent = Model.belongsTo(models.Agent, {
      foreignKey: 'agentId',
    });

    Model.Bots = Model.hasMany(models.Bot, {
      foreignKey: 'userId',
    });
  };

  return Model;
};

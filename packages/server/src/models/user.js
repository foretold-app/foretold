module.exports = (sequelize, DataTypes) => {
  const User = sequelize.define('User', {
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
        is: ['^[a-z0-9._]{0,30}$', 'i'],
      },
    },
    description: {
      type: DataTypes.STRING(255),
      allowNull: true,
    },
    auth0Id: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    email: {
      type: DataTypes.STRING(64),
      allowNull: true,
      validate: {
        isEmail: true,
      },
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
    createdAt: {
      type: DataTypes.DATE,
      defaultValue: DataTypes.NOW,
    },
    updatedAt: {
      type: DataTypes.DATE,
      defaultValue: DataTypes.NOW,
    },
  });

  User.associate = function associate(models) {
    User.Agent = User.belongsTo(models.Agent, {
      foreignKey: 'agentId',
    });

    User.Bots = User.hasMany(models.Bot, {
      foreignKey: 'userId',
    });
  };

  return User;
};

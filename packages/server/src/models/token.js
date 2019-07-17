const { TOKEN_TYPE } = require('./enums/token-type');

module.exports = (sequelize, DataTypes) => {
  const Token = sequelize.define('Token', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: sequelize.fn('uuid_generate_v4'),
      allowNull: false,
    },
    agentId: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    token: {
      type: DataTypes.STRING,
      allowNull: true,
    },
    isActive: {
      type: DataTypes.BOOLEAN,
      allowNull: false,
      defaultValue: true,
    },
    type: {
      type: DataTypes.ENUM([
        TOKEN_TYPE.ACCESS_TOKEN,
        TOKEN_TYPE.AUTH_TOKEN,
      ]),
      defaultValue: TOKEN_TYPE.ACCESS_TOKEN,
    },
    usageCount: {
      type: DataTypes.INTEGER,
      allowNull: true,
    },
    expiresAt: {
      type: DataTypes.DATE,
      defaultValue: DataTypes.NOW,
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

  Token.associate = function associate(models) {
    Token.Agent = Token.belongsTo(models.Agent, {
      foreignKey: 'agentId',
    });
  };

  return Token;
};

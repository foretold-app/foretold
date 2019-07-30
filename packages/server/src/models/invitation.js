const { INVITATION_STATUS } = require('./enums/invitation-status');

module.exports = (sequelize, DataTypes) => {
  const Invitation = sequelize.define('Invitation', {
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
    channelId: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    status: {
      type: DataTypes.ENUM([
        INVITATION_STATUS.AWAITING,
        INVITATION_STATUS.ACCEPTED,
      ]),
      defaultValue: INVITATION_STATUS.AWAITING,
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

  Invitation.associate = function associate(models) {
    Invitation.Agent = Invitation.belongsTo(models.Agent, {
      foreignKey: 'agentId',
    });
  };

  return Invitation;
};

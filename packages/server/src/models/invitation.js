const { INVITATION_STATUS } = require('../enums');

module.exports = (sequelize, DataTypes) => {
  const Invitation = sequelize.define('Invitation', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: sequelize.fn('uuid_generate_v4'),
      allowNull: false,
    },
    email: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    inviterAgentId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    channelId: {
      type: DataTypes.UUID(),
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
      defaultValue: sequelize.fn('statement_timestamp'),
    },
    updatedAt: {
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
    },
  });

  /**
   * @todo: fix it, remove it.
   * @deprecated
   * @param models
   */
  Invitation.associate = function associate(models) {
    Invitation.Inviter = Invitation.belongsTo(models.Agent, {
      foreignKey: 'inviterAgentId',
    });
    Invitation.Channel = Invitation.belongsTo(models.Channel, {
      foreignKey: 'channelId',
    });
  };

  return Invitation;
};

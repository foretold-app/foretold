const moment = require('moment');

const { MEASURABLE_STATE } = require('../../../enums');
const { MEASURABLE_VALUE_TYPE } = require('../../../enums');

module.exports = (sequelize, DataTypes) => {
  const Measurable = sequelize.define('Measurable', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: DataTypes.UUIDV4,
      allowNull: false,
    },

    // Meta
    name: {
      type: DataTypes.STRING,
      allowNull: false,
      get: getName,
    },
    labelSubject: {
      type: DataTypes.STRING,
      allowNull: true,
    },
    labelOnDate: {
      type: DataTypes.DATE,
      allowNull: true,
    },
    labelProperty: {
      type: DataTypes.STRING,
      allowNull: true,
    },
    labelCustom: {
      type: DataTypes.TEXT,
      allowNull: true,
    },
    labelStartAtDate: {
      type: DataTypes.DATE,
      allowNull: true,
    },
    labelEndAtDate: {
      type: DataTypes.DATE,
      allowNull: true,
    },
    labelConditionals: {
      type: DataTypes.ARRAY(DataTypes.STRING),
      allowNull: true,
    },
    valueType: {
      type: DataTypes.ENUM([
        MEASURABLE_VALUE_TYPE.FLOAT,
        MEASURABLE_VALUE_TYPE.DATE,
        MEASURABLE_VALUE_TYPE.PERCENTAGE,
        MEASURABLE_VALUE_TYPE.UNRESOLVED,
      ]),
      allowNull: false,
    },
    min: {
      type: DataTypes.FLOAT,
      allowNull: true,
    },
    max: {
      type: DataTypes.FLOAT,
      allowNull: true,
    },

    // State
    state: {
      type: DataTypes.STRING,
      defaultValue: MEASURABLE_STATE.OPEN,
      allowNull: false,
    },
    stateUpdatedAt: {
      allowNull: true,
      type: DataTypes.DATE,
    },

    // Status
    isArchived: {
      type: DataTypes.BOOLEAN,
      allowNull: false,
      defaultValue: false,
    },

    // Resolution
    resolutionEndpoint: {
      type: DataTypes.TEXT,
      allowNull: true,
    },

    // We default to having questions resolve 1 month after they are created.
    expectedResolutionDate: {
      allowNull: true,
      type: DataTypes.DATE,
      // @todo: This should be function.
      defaultValue: moment().add(1, 'months').format('MMM DD, YYYY HH:mm'),
    },

    // Links
    channelId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    seriesId: {
      type: DataTypes.UUID(),
      allowNull: true,
    },
    // @entity: creator-link
    creatorId: {
      type: DataTypes.UUID(),
      allowNull: true,
    },

    // Object Dates
    createdAt: {
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
      allowNull: false,
    },
    updatedAt: {
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
      allowNull: false,
    },
  });

  /**
   * @todo: To fix, remove this code from this layer.
   * @todo: It leads the architecture of the app to circular dependency
   * @returns {string}
   */
  async function getName() {
    if (this.labelSubject && this.labelProperty) {
      // @todo: To fix require.
      const { GlobalSettingsData } = require('../../index');
      const globalSettings = new GlobalSettingsData();
      const kenFacade = await globalSettings.getKen();
      const names = kenFacade.names(
        this.labelSubject,
        this.labelProperty,
      );
      return `${names.subject} ${names.property}`;
    }

    if (this.dataValues.name) {
      return this.dataValues.name;
    }

    return '';
  }

  /**
   * @todo: To rework (to remove).
   * @return {Promise<Models.Measurable>}
   */
  Measurable.prototype.archive = async function archive() {
    await this.update({ isArchived: true });
  };

  /**
   * @todo: To rework (to remove).
   * @return {Promise<Models.Measurable>}
   */
  Measurable.prototype.unarchive = async function unarchive() {
    await this.update({ isArchived: false });
  };

  /**
   * @todo: fix it, remove it!
   * @deprecated
   * @param models
   */
  Measurable.associate = function associate(models) {
    Measurable.Creator = Measurable.belongsTo(models.Agent, {
      foreignKey: 'creatorId',
      as: 'creator',
    });

    // Usage:
    // const me = await models.Measurable.find();
    // const ch = await me.getChannel();
    Measurable.Channel = Measurable.belongsTo(models.Channel, {
      foreignKey: 'channelId',
    });
  };

  return Measurable;
};

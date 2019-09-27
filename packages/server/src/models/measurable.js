const Sequelize = require('sequelize');

const { MEASURABLE_STATE } = require('../enums/measurable-state');
const { MEASURABLE_VALUE_TYPE } = require('../enums/measurable-value-type');

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
    expectedResolutionDate: {
      allowNull: true,
      type: DataTypes.DATE,
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
    creatorId: {
      type: DataTypes.UUID(),
      allowNull: true,
    },

    // Object Dates
    createdAt: {
      type: DataTypes.DATE,
      defaultValue: DataTypes.NOW,
    },
    updatedAt: {
      type: DataTypes.DATE,
      defaultValue: DataTypes.NOW,
    },
  });

  /**
   * @return {Promise<Models.Measurable>}
   */
  Measurable.prototype.archive = async function archive() {
    await this.update({ isArchived: true });
  };

  /**
   * @return {Promise<Models.Measurable>}
   */
  Measurable.prototype.unarchive = async function unarchive() {
    await this.update({ isArchived: false });
  };

  /**
   * @return {Promise<Models.Measurable>}
   */
  Measurable.prototype.open = async function open() {
    await this.updateState(MEASURABLE_STATE.OPEN);
  };

  /**
   * @return {Promise<Models.Measurable>}
   */
  Measurable.prototype.judged = async function judged() {
    await this.updateState(MEASURABLE_STATE.JUDGED);
  };

  /**
   * @return {Promise<Models.Measurable>}
   */
  Measurable.prototype.judgementPending = async function judgementPending() {
    await this.updateState(MEASURABLE_STATE.JUDGEMENT_PENDING);
  };

  /**
   * @return {Promise<Models.Measurable>}
   */
  Measurable.prototype.closedAsUnresolved = async function closedAsUnresolved() {
    await this.updateState(MEASURABLE_STATE.CLOSED_AS_UNRESOLVED);
  };

  /**
   * @param {string} state
   * @return {Promise<Models.Measurable>}
   */
  Measurable.prototype.updateState = async function updateState(state) {
    await this.update({ state, stateUpdatedAt: Sequelize.fn('now') });
  };

  Measurable.associate = function associate(models) {
    Measurable.Measurements = Measurable.hasMany(models.Measurement, {
      foreignKey: 'measurableId',
      as: 'Measurements',
    });

    Measurable.Series = Measurable.belongsTo(models.Series, {
      foreignKey: 'seriesId',
      as: 'series',
    });

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

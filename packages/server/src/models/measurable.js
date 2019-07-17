const Sequelize = require('sequelize');
const _ = require('lodash');
const fetch = require("node-fetch");
const moment = require('moment');

const { clientUrl } = require('../lib/urls');

const { MEASURABLE_STATE } = require('./enums/measurable-state');
const { MEASURABLE_VALUE_TYPE } = require('./enums/measurable-value-type');
const { MEASUREMENT_COMPETITOR_TYPE } = require('./enums/measurement-competitor-type');
const { MEASUREMENT_VALUE } = require('./enums/measurement-value');

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

    // State
    state: {
      type: DataTypes.STRING,
      defaultValue: MEASURABLE_STATE.OPEN,
      allowNull: false,
    },
    stateUpdatedAt: {
      allowNull: true,
      type: DataTypes.DATE
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
      type: DataTypes.DATE
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

    // Counts
    measurementCount: {
      allowNull: true,
      type: Sequelize.VIRTUAL(DataTypes.INTEGER),
      get: getMeasurementCount,
    },
    measurerCount: {
      allowNull: true,
      type: Sequelize.VIRTUAL(DataTypes.INTEGER),
      get: getMeasurerCount,
    },

    // Satellite
    resolutionEndpointResponse: {
      allowNull: true,
      type: Sequelize.VIRTUAL(DataTypes.FLOAT),
      get: resolutionEndpointResponseGetter,
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

  Measurable.addHook('beforeUpdate', async (instance) => {
    await watchExpectedResolutionDate(instance);
  });

  async function getMeasurementCount() {
    const items = await this.getMeasurements();
    return items.length;
  }

  async function getMeasurerCount() {
    const items = await this.getMeasurements();
    return _.uniq(items.map(i => i.agentId)).length;
  }

  async function resolutionEndpointResponseGetter() {
    const endpoint = await this.dataValues.resolutionEndpoint;
    if (!endpoint) return false;
    try {
      const response = await fetch(endpoint);
      const json = await response.json();
      const match = JSON.stringify(json).match(/[-+]?[0-9]*\.?[0-9]+/);
      const asFloat = parseFloat(match[0]);

      console.log(
        `got response from endpoint. Url: ${endpoint}, ` +
        `Response: ${JSON.stringify(json)}, Float: ${asFloat}`
      );

      return asFloat;
    } catch (e) {
      console.error(`Error getting response from endpoint. ` +
        `Url: ${endpoint}, error: ${e}`);
      return null;
    }
  }

  async function watchExpectedResolutionDate(instance) {
    const isChanged = instance.changed('expectedResolutionDate');
    if (!isChanged) return;
    const current = instance.getDataValue('expectedResolutionDate');
    if (!current) return;
    const now = new Date();
    const isResolutionDateInFuture = current >= now;
    if (isResolutionDateInFuture) {
      instance.set('state', MEASURABLE_STATE.OPEN);
    }
  }

  Measurable.needsResolutionResponse = async function needsResolutionResponse() {
    return Measurable.findAll({
      where: {
        state: MEASURABLE_STATE.JUDGEMENT_PENDING,
        expectedResolutionDate: {
          [Sequelize.Op.lt]: Sequelize.fn('now'),
        },
      },
    });
  };

  Measurable.needsToBePending = async function needsToBePending() {
    return Measurable.findAll({
      where: {
        state: MEASURABLE_STATE.OPEN,
        [Sequelize.Op.or]: [
          {
            expectedResolutionDate: {
              [Sequelize.Op.lt]: Sequelize.fn('now'),
            },
          },
          { expectedResolutionDate: null },
        ],
      },
    });
  };

  Measurable.prototype.updateState = async function updateState(state) {
    await this.update({ state, stateUpdatedAt: Sequelize.fn('now') });
  };

  Measurable.prototype.archive = async function archive() {
    await this.update({ isArchived: true });
  };

  Measurable.prototype.unarchive = async function unarchive() {
    await this.update({ isArchived: false });
  };

  Measurable.prototype.judged = async function judged() {
    await this.updateState(MEASURABLE_STATE.JUDGED);
  };

  Measurable.prototype.judgementPending = async function judgementPending() {
    await this.updateState(MEASURABLE_STATE.JUDGEMENT_PENDING);
  };

  /**
   * @todo: implement client for this code
   * @todo: do not remove
   * @param {Models.Agent.id} agentId
   * @return {Promise<void>}
   */
  Measurable.prototype.processResolution =
    async function processResolution(agentId) {
      const asFloat = await this.resolutionEndpointResponse;
      if (asFloat) {
        await sequelize.models.Measurement.create({
          agentId,
          competitorType: MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE,
          measurableId: this.dataValues.id,
          value: {
            dataType: MEASUREMENT_VALUE.floatPoint,
            data: asFloat,
          },
        });
      }
      await this.judged();
    };

  /**
   * @todo: move me
   * @public
   * @param {Models.Creator} creator
   * @return {Promise<*>}
   */
  Measurable.prototype.getCreationNotification =
    async function getCreationNotification(creator) {
      const agent = await creator.getAgent();
      return {
        attachments: [{
          pretext: "New Measurable Created",
          title: this.name,
          title_link: `${clientUrl}/c/${this.channelId}`,
          author_name: creator.name,
          author_link: `${clientUrl}/agents/${agent.id}`,
          text: this.labelCustom,
          fields: [
            {
              title: "Resolution Date",
              value: moment(this.expectedResolutionDate).format("MMM DD, YYYY"),
              short: true,
            },
          ],
          color: "#4a8ed8",
        }],
      };
    };

  /**
   * @param {object} ops
   * @return {string[]}
   */
  Measurable.prototype.changedFields = function changedFields(ops) {
    return Object.keys(ops)
      .filter(r => r !== "expectedResolutionDate")
      .filter(r => this[r] !== ops[r]);
  };

  /**
   * @todo: move me
   * @public
   * @param {Models.Creator} creator
   * @param {object} newData
   * @return {Promise<*>}
   */
  Measurable.prototype.getUpdateNotifications =
    async function getUpdateNotifications(creator, newData) {
      const changed = this.changedFields(newData);
      const agent = await creator.getAgent();
      return {
        attachments: [{
          pretext: "Measurable Updated",
          title: this.name,
          title_link: `${clientUrl}/c/${this.channelId}`,
          author_name: creator.name,
          author_link: `${clientUrl}/agents/${agent.id}`,
          fields: changed.map(c => ({
            title: c,
            short: false,
            value: `*From*: ${this[c]} \n*To*:  ${newData[c]}`
          })),
          color: "#ffe75e",
        }],
      };
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

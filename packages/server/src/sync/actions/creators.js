const _ = require('lodash');

const { AgentsData } = require('../../data');
const { MeasurablesData } = require('../../data');
const { ChannelMembershipsData } = require('../../data');
const logger = require('../../lib/log');

const { Data } = require('../../data/classes');
const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');

const { MEASURABLE_VALUE_TYPE } = require('../../enums');
const { CHANNEL_MEMBERSHIP_ROLES } = require('../../enums');
const { AGENT_TYPE } = require('../../enums');
const { MEASURABLE_STATE } = require('../../enums');

class Creators {
  constructor() {
    this.agents = new AgentsData();
    this.measurables = new MeasurablesData();
    this.channelMemberships = new ChannelMembershipsData();
    this.log = logger.module('sync/actions/creators');
  }

  /**
   * @param {Defs.Series} series
   * @returns {Promise<void>}
   */
  async createMeasurables(series) {
    await this._multiplyArraysToCreateMeasurable(
      series.subjects,
      series.properties,
      series.dates,
      series,
    );
  }

  /**
   * @param {*[]} subjects
   * @param {*[]} properties
   * @param {*[]} dates
   * @param {Defs.Series} series
   * @returns {Promise<void>}
   * @private
   */
  async _multiplyArraysToCreateMeasurable(subjects, properties, dates, series) {
    for (const subject of subjects) {
      for (const property of properties) {
        for (const date of dates) {
          console.log('Making Measurable for Series:',
            subject, property, date);
          await this._createMeasurable(subject, property, date, series);
        }
      }
    }
  }

  /**
   * @param {Defs.Series} series
   * @returns {Promise<void>}
   */
  async createNewMeasurables(series) {
    const previousSubjects = series.previous('subjects');
    const previousProperties = series.previous('properties');
    const previousDates = series.previous('dates');

    const currentSubjects = series.get('subjects');
    const currentProperties = series.get('properties');
    const currentDates = series.get('dates');

    // currentSubjects - previousSubjects
    const newSubjects = _.difference(currentSubjects, previousSubjects);
    if (newSubjects.length > 0) {
      await this._multiplyArraysToCreateMeasurable(
        newSubjects, // <--
        currentProperties,
        currentDates,
        series,
      );
    }

    // currentProperties - previousProperties
    const newProperties = _.difference(currentProperties, previousProperties);
    if (newProperties.length > 0) {
      await this._multiplyArraysToCreateMeasurable(
        currentSubjects,
        newProperties, // <--
        currentDates,
        series,
      );
    }

    // currentDates - previousDates
    const newDates = _.differenceBy(
      currentDates,
      previousDates,
      (e) => e.toISOString(),
    );
    if (newDates.length > 0) {
      await this._multiplyArraysToCreateMeasurable(
        currentSubjects,
        currentProperties,
        newDates, // <--
        series,
      );
    }
  }

  /**
   * @param {string} subject
   * @param {string} property
   * @param {string} date
   * @param {Defs.Series} series
   * @returns {Promise<void>}
   * @private
   */
  async _createMeasurable(subject, property, date, series) {
    const data = new Data({
      name: '',
      labelSubject: subject,
      labelProperty: property,
      labelOnDate: date,
      expectedResolutionDate: date,
      seriesId: series.id,
      creatorId: series.creatorId,
      channelId: series.channelId,
      valueType: MEASURABLE_VALUE_TYPE.FLOAT,
    });
    await this.measurables.createOne(data);
  }

  /**
   * @param {Defs.Channel} channel
   * @returns {Promise<boolean>}
   */
  async createChannelMembership(channel) {
    const params = new Params({
      channelId: channel.id,
      agentId: channel.creatorId,
    });
    const query = new Query({});
    const data = new Data({
      role: CHANNEL_MEMBERSHIP_ROLES.ADMIN,
      channelId: channel.id,
      agentId: channel.creatorId,
    });
    await this.channelMemberships.upsertOne(params, query, data);
    return true;
  }

  /**
   * @param {Defs.Bot} instance
   * @returns {Promise<boolean>}
   */
  async createBotAgent(instance) {
    const data = new Data({ type: AGENT_TYPE.BOT });
    const agent = await this.agents.createOne(data);
    instance.agentId = agent.id;
    return true;
  }

  /**
   * @param {Defs.User} instance
   * @returns {Promise<boolean>}
   */
  async createUserAgent(instance) {
    const data = new Data({ type: AGENT_TYPE.USER });
    const agent = await this.agents.createOne(data);
    instance.agentId = agent.id;
    return true;
  }

  /**
   * @param {Defs.Measurable} instance
   * @returns {Promise<boolean>}
   */
  async checkMeasurableState(instance) {
    if (instance.changed('expectedResolutionDate')) {
      if (instance.expectedResolutionDate >= new Date()) {
        await instance.set('state', MEASURABLE_STATE.OPEN);
      }
    }
    return true;
  }

  /**
   * @param {Defs.Measurement} instance
   * @returns {Promise<boolean>}
   */
  async checkMeasurement(instance) {
    if (instance.relevantAt == null) {
      instance.relevantAt = Date.now();
    }
    return true;
  }
}

module.exports = {
  Creators,
};

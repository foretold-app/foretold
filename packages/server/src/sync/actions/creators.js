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

const log = logger.module('sync/actions/creators');

class Creators {
  constructor() {
    this.agents = new AgentsData();
    this.measurables = new MeasurablesData();
    this.channelMemberships = new ChannelMembershipsData();
  }

  /**
   * @param {Defs.Series} series
   * @returns {Promise<void>}
   */
  async createMeasurables(series) {
    for (const subject of series.subjects) {
      for (const property of series.properties) {
        for (const date of series.dates) {
          console.log(
            'Making Measurable for Series:',
            subject,
            property,
            date,
          );
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
    log.trace(series.previous('subjects'), series.get('subjects'));
    log.trace(series.previous('properties'), series.get('properties'));
    log.trace(series.previous('dates'), series.get('dates'));
    // for (const subject of series.subjects) {
    //   for (const property of series.properties) {
    //     for (const date of series.dates) {
    //       console.log(
    //         'Making Measurable for Series:',
    //         subject,
    //         property,
    //         date,
    //       );
    //       await this._createMeasurable(subject, property, date, series);
    //     }
    //   }
    // }
  }

  /**
   * @param subject
   * @param property
   * @param date
   * @param series
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

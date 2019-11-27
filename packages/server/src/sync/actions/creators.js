const { AgentsData } = require('../../data');
const { MeasurablesData } = require('../../data');
const { ChannelMembershipsData } = require('../../data');
const { Data } = require('../../data/classes');

const { MEASURABLE_VALUE_TYPE } = require('../../enums');
const { CHANNEL_MEMBERSHIP_ROLES } = require('../../enums');
const { AGENT_TYPE } = require('../../enums');

class Creators {
  constructor() {
    this.agents = new AgentsData();
    this.measurables = new MeasurablesData();
    this.channelMemberships = new ChannelMembershipsData();
  }

  /**
   * @param {Models.Series} series
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
          await this.measurables.createOne({
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
        }
      }
    }
  }

  /**
   * @param {Models.Channel} channel
   * @returns {Promise<boolean>}
   */
  async createChannelMembership(channel) {
    await this.channelMemberships.upsertOne({
      channelId: channel.id,
      agentId: channel.creatorId,
    }, {}, {
      role: CHANNEL_MEMBERSHIP_ROLES.ADMIN,
      channelId: channel.id,
      agentId: channel.creatorId,
    });
    return true;
  }

  /**
   * @param {Models.Bot} instance
   * @returns {Promise<boolean>}
   */
  async createBotAgent(instance) {
    const data = new Data({ type: AGENT_TYPE.BOT });
    const agent = await this.agents.createOne(data);
    instance.agentId = agent.id;
    return true;
  }

  /**
   * @param {Models.User} instance
   * @returns {Promise<boolean>}
   */
  async createUserAgent(instance) {
    const data = new Data({ type: AGENT_TYPE.USER });
    const agent = await this.agents.createOne(data);
    instance.agentId = agent.id;
    return true;
  }
}

module.exports = {
  Creators,
};

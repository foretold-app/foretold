const _ = require('lodash');
const moment = require('moment');

const data = require('../../data');
const { notify } = require('../../lib/notifications');
const { clientUrl } = require('../../lib/urls');

class Notifications {
  constructor() {
    this.data = data;
  }

  /**
   * @public
   * @param {Models.Measurement} measurement
   * @returns {Promise<boolean>}
   */
  async newMeasurement(measurement) {
    const measurable = await measurement.getMeasurable();
    const channel = await measurable.getChannel();
    if (!channel.isPublic) return false;

    const agent = await measurement.getAgent();

    const notification = await this.getNotificationNewMeasurement(
      measurement,
      measurable,
      agent,
    );

    await notify(notification);
    return true;
  }

  /**
   * @public
   * @param {Models.Measurable} measurable
   * @return {Promise<boolean>}
   */
  async newMeasurable(measurable) {
    const channel = await measurable.getChannel();
    if (channel.isPublic) return false;

    const agent = await measurable.getCreator();
    const notification = await this.getNotificationNewMeasurable(
      measurable,
      agent,
    );

    await notify(notification);
    return true;
  }

  /**
   * @public
   * @param {Models.Measurable} measurable
   * @return {Promise<boolean>}
   */
  async updateMeasurable(measurable) {
    const channel = await measurable.getChannel();
    if (channel.isPublic) return false;

    const agent = await measurable.getCreator();
    const notification = await this.getNotificationUpdateMeasurable(
      measurable,
      agent,
      measurable,
    );

    await notify(notification);
    return true;
  }

  /**
   * @protected
   * @param {Models.Measurement} measurement
   * @param {Models.Measurable} measurable
   * @param {Models.Agent} agent
   * @returns {Promise<*>}
   */
  async getNotificationNewMeasurement(measurement, measurable, agent) {
    return {
      attachments: [{
        pretext: 'New Measurement Created',
        title: measurable.name,
        title_link: `${clientUrl}/c/${measurable.channelId}`,
        author_name: agent.name,
        author_link: `${clientUrl}/agents/${agent.id}`,
        text: measurement.description,
        fields: [
          {
            title: 'Type',
            value: measurement.competitorType,
            short: true,
          },
        ],
        color: '#d2ebff',
      }],
    };
  }

  /**
   * @protected
   * @param {Models.Measurable} measurable
   * @param {Models.Creator} agent
   * @return {Promise<*>}
   */
  async getNotificationNewMeasurable(measurable, agent) {
    return {
      attachments: [{
        pretext: 'New Measurable Created',
        title: measurable.name,
        title_link: `${clientUrl}/c/${measurable.channelId}`,
        author_name: agent.name,
        author_link: `${clientUrl}/agents/${agent.id}`,
        text: measurable.labelCustom,
        fields: [
          {
            title: 'Resolution Date',
            value: moment(measurable.expectedResolutionDate)
              .format('MMM DD, YYYY'),
            short: true,
          },
        ],
        color: '#4a8ed8',
      }],
    };
  };

  /**
   * @protected
   * @param {Models.Measurable} measurable
   * @param {Models.Creator} agent
   * @param {object} newData
   * @return {Promise<*>}
   */
  async getNotificationUpdateMeasurable(measurable, agent, newData) {
    const changed = this.changedFields(measurable, newData);
    return {
      attachments: [{
        pretext: 'Measurable Updated',
        title: measurable.name,
        title_link: `${clientUrl}/c/${measurable.channelId}`,
        author_name: agent.name,
        author_link: `${clientUrl}/agents/${agent.id}`,
        fields: changed.map((c) => ({
          title: c,
          short: false,
          value: `*From*: ${measurable[c]} \n*To*:  ${newData[c]}`,
        })),
        color: '#ffe75e',
      }],
    };
  };

  /**
   * @protected
   * @param {Models.Measurable} measurable
   * @param {object} ops
   * @return {string[]}
   */
  changedFields(measurable, ops) {
    return Object.keys(ops)
      .filter((r) => r !== 'expectedResolutionDate')
      .filter((r) => measurable[r] !== ops[r]);
  };
}

module.exports = {
  Notifications,
};

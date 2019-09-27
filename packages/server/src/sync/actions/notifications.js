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
   * @param {Models.Measurement} measurement
   * @returns {Promise<boolean>}
   */
  async newMeasurement(measurement) {
    const agent = await measurement.getAgent();
    const measurable = await measurement.getMeasurable();
    const channel = await measurable.getChannel();

    if (!channel.isPublic) return false;

    const notification = await this.getNotificationNewMeasurement(
      measurement,
      measurable,
      agent,
    );

    await notify(notification);
    return true;
  }

  /**
   * @param entity
   * @param entity.getCreationNotification
   * @param entity.getChannel
   * @param creator
   * @return {Promise<boolean>}
   */
  async creationNotification(entity, creator) {
    const notification = await entity.getCreationNotification(creator);
    const channel = await entity.getChannel();
    if (channel.isPublic) await notify(notification);
    return true;
  }

  /**
   * @param entity
   * @param entity.getUpdateNotifications
   * @param entity.getChannel
   * @param creator
   * @param data
   * @return {Promise<boolean>}
   */
  async updateNotification(entity, creator, data) {
    const notification = await entity.getUpdateNotifications(creator, data);
    const channel = await entity.getChannel();
    if (channel.isPublic) await notify(notification);
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
            value: moment(measurable.expectedResolutionDate).format('MMM DD, YYYY'),
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

const _ = require('lodash');
const moment = require('moment');

const data = require('../../data');
const { sendNotificationToSlack } = require('../../lib/slack');
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

    await sendNotificationToSlack(notification);
    return true;
  }

  /**
   * @public
   * @param {Models.Measurable} measurable
   * @return {Promise<boolean>}
   */
  async newMeasurable(measurable) {
    const channel = await measurable.getChannel();
    if (!channel.isPublic) return false;

    const agent = await measurable.getCreator();
    const notification = await this.getNotificationNewMeasurable(
      measurable,
      agent,
    );

    await sendNotificationToSlack(notification);
    return true;
  }

  /**
   * @public
   * @param {Models.Measurable} measurable
   * @return {Promise<boolean>}
   */
  async updateMeasurable(measurable) {
    const channel = await measurable.getChannel();
    if (!channel.isPublic) return false;

    const agent = await measurable.getCreator();
    const notification = await this.getNotificationUpdateMeasurable(
      measurable,
      agent,
    );

    await sendNotificationToSlack(notification);
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
        author_name: await agent.name,
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
        author_name: await agent.name,
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
  }

  /**
   * @protected
   * @param {Models.Measurable} measurable
   * @param {Models.Creator} agent
   * @return {Promise<*>}
   */
  async getNotificationUpdateMeasurable(measurable, agent) {
    const changed = this.changedFields(measurable);
    return {
      attachments: [{
        pretext: 'Measurable Updated',
        title: measurable.name,
        title_link: `${clientUrl}/c/${measurable.channelId}`,
        author_name: await agent.name,
        author_link: `${clientUrl}/agents/${agent.id}`,
        fields: changed,
        color: '#ffe75e',
      }],
    };
  }

  /**
   * @todo: temporary turned off
   * @protected
   * @param {Models.Measurable} measurable
   * @return {string[]}
   */
  changedFields(measurable) {
    return [];
    const fields = [
      'name',
      'labelCustom',
      'valueType',
      'createdAt',
      'updatedAt',
      'creatorId',
      'resolutionEndpoint',
      'state',
      'stateUpdatedAt',
      'labelSubject',
      'labelOnDate',
      'labelProperty',
      'seriesId',
      'channelId',
      'isArchived',
      'min',
      'max',
    ];
    return fields
      .filter((r) => measurable.get(r) !== measurable.previous(r))
      .map((r) => ({
        title: r,
        short: false,
        value: `*From*: ${measurable.previous(r)} \n*To*:  ${measurable.get(r)}`,
      }));
  }
}

module.exports = {
  Notifications,
};

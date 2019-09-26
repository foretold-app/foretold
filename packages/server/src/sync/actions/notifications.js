const _ = require('lodash');

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
}

module.exports = {
  Notifications,
};

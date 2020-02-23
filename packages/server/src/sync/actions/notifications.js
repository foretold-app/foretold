const moment = require('moment');

const { sendNotificationToSlack } = require('../../lib/slack');
const { clientUrl } = require('../../../config/client-urls');

class Notifications {
  /**
   * @public
   * @param {Defs.Measurement} measurement
   * @returns {Promise<boolean>}
   */
  async newMeasurementSlackNotification(measurement) {
    const measurable = await measurement.getMeasurable();
    const channel = await measurable.getChannel();

    if (!channel.isPublic) {
      return false;
    }

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
   * @param {Defs.Measurable} measurable
   * @return {Promise<boolean>}
   */
  async newMeasurableSlackNotification(measurable) {
    const channel = await measurable.getChannel();

    if (!channel.isPublic) {
      return false;
    }

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
   * @param {Defs.Measurable} measurable
   * @return {Promise<boolean>}
   */
  async updateMeasurableSlackNotification(measurable) {
    const channel = await measurable.getChannel();

    if (!channel.isPublic) {
      return false;
    }

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
   * @param {Defs.Measurement} measurement
   * @param {Defs.Measurable} measurable
   * @param {Defs.Agent} agent
   * @returns {Promise<*>}
   */
  async getNotificationNewMeasurement(measurement, measurable, agent) {
    return {
      attachments: [{
        pretext: 'New Measurement Created',
        title: await measurable.name,
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
   * @param {Defs.Measurable} measurable
   * @param {Defs.Creator} agent
   * @return {Promise<*>}
   */
  async getNotificationNewMeasurable(measurable, agent) {
    return {
      attachments: [{
        pretext: 'New Measurable Created',
        title: await measurable.name,
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
   * @param {Defs.Measurable} measurable
   * @param {Defs.Creator} agent
   * @return {Promise<*>}
   */
  async getNotificationUpdateMeasurable(measurable, agent) {
    const changed = this.changedFields(measurable);
    return {
      attachments: [{
        pretext: 'Measurable Updated',
        title: await measurable.name,
        title_link: `${clientUrl}/c/${measurable.channelId}`,
        author_name: await agent.name,
        author_link: `${clientUrl}/agents/${agent.id}`,
        fields: changed,
        color: '#ffe75e',
      }],
    };
  }

  /**
   * @protected
   * @param {Defs.Measurable} _measurable
   * @return {string[]}
   */
  changedFields(_measurable) {
    return [];
  }
}

module.exports = {
  Notifications,
};

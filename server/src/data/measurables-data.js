const Sequelize = require('sequelize');

const models = require("../models");
const { notify } = require("../lib/notifications");

class MeasurablesData {

  /**
   * @param root
   * @param values
   * @param options
   * @return {Promise<*>}
   */
  async createMeasurable(root, values, options) {
    const user = options.user;
    const newMeasurable = await models.Measurable.create({
      ...values,
      creatorId: user.agentId,
    });
    let notification = await newMeasurable.creationNotification(user);
    notify(notification);
    return newMeasurable;
  }

  /**
   * @param root
   * @param values
   * @param options
   * @return {Promise<Promise<void>|Promise<WebAPICallResult>|never>}
   */
  async archiveMeasurable(root, values, options) {
    const { id } = values;
    const user = options.user;
    let measurable = await models.Measurable.findById(id);
    if (measurable.creatorId !== user.agentId) {
      throw new Error("User does not have permission");
    }
    return measurable.archive();
  }

  /**
   * @param root
   * @param values
   * @param options
   * @return {Promise<Promise<WebAPICallResult> | Promise<void>>}
   */
  async unArchiveMeasurable(root, values, options) {
    const { id } = values;
    const user = options.user;
    let measurable = await models.Measurable.findById(id);
    if (measurable.creatorId !== user.agentId) {
      throw new Error("User does not have permission");
    }
    return measurable.unarchive();
  }

  /**
   * @param root
   * @param values
   * @param options
   * @return {Promise<Promise<WebAPICallResult> | Promise<void>>}
   */
  async editMeasurable(root, values, options) {
    const {
      id,
      name,
      description,
      expectedResolutionDate,
      descriptionEntity,
      descriptionDate,
      resolutionEndpoint,
      descriptionProperty
    } = values;
    const user = options.user;
    let measurable = await models.Measurable.findById(id);
    if (measurable.creatorId !== user.agentId) {
      throw new Error("User does not have permission");
    }
    let notification = await measurable.updateNotifications(user, {
      name,
      description,
      expectedResolutionDate,
      resolutionEndpoint,
      descriptionEntity,
      descriptionDate,
      descriptionProperty
    });
    notify(notification);
    return measurable.update({
      name,
      description,
      expectedResolutionDate,
      resolutionEndpoint,
      descriptionEntity,
      descriptionDate,
      descriptionProperty
    });
  }

  /**
   * @param {object} options
   * @return {Promise<*|Array<Model>>}
   */
  async getAll(options) {
    const { offset, limit, channelId, seriesId, creatorId } = options;

    let where = {
      state: {
        [Sequelize.Op.ne]: "ARCHIVED"
      }
    };

    if (seriesId) {
      where.seriesId = { [Sequelize.Op.eq]: seriesId };
    }
    if (creatorId) {
      where.creatorId = { [Sequelize.Op.eq]: creatorId };
    }
    if (channelId) {
      where.channelId = { [Sequelize.Op.eq]: channelId };
    }

    const items = await models.Measurable.findAll({
      limit: limit,
      offset: offset,
      order: [['createdAt', 'DESC']],
      where
    });

    return items;
  }

}

module.exports = {
  MeasurablesData,
};

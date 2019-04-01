const _ = require('lodash');

const models = require("../models");
const { notify } = require("../lib/notifications");

const { DataBase } = require('./data-base');

class MeasurablesData extends DataBase {

  /**
   * @param data
   * @return {Promise<*>}
   */
  async createMeasurable(data, user) {
    const newMeasurable = await models.Measurable.create(data);
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
    if (_.get(measurable, 'creatorId') !== _.get(user, 'agentId')) {
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
    if (_.get(measurable, 'creatorId') !== _.get(user, 'agentId')) {
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
    if (_.get(measurable, 'creatorId') !== _.get(user, 'agentId')) {
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
        [models.sequelize.Op.ne]: "ARCHIVED"
      },
      $and: [{ channelId: { $in: this.channelIdsLiteral(options.agentId) } }],
    };

    if (seriesId) {
      where.seriesId = { [models.sequelize.Op.eq]: seriesId };
    }
    if (creatorId) {
      where.creatorId = { [models.sequelize.Op.eq]: creatorId };
    }
    if (channelId) {
      where.$and.push({ channelId });
    }

    const items = await models.Measurable.findAll({
      limit,
      offset,
      where,
      order: [['createdAt', 'DESC']],
    });

    return items;
  }

  /**
   * @param {string} id
   * @param {object} options
   * @param {string} options.agentId
   * @return {Promise<*>}
   */
  async getOne(id, options = {}) {
    const restrictions = 'agentId' in options
      ? { channelId: { $in: this.channelIdsLiteral(options.agentId) } }
      : {};
    return await models.Measurable.findOne({
      where: {
        id,
        ...restrictions,
      }
    });
  }

}

module.exports = {
  MeasurablesData,
};

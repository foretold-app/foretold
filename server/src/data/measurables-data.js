const _ = require('lodash');

const models = require("../models");
const { notify } = require("../lib/notifications");

const { DataBase } = require('./data-base');

class MeasurablesData extends DataBase {

  /**
   * @param data
   * @param {object} user
   * @return {Promise<*>}
   */
  async createOne(data, user) {
    const newMeasurable = await models.Measurable.create(data);
    let notification = await newMeasurable.creationNotification(user);
    notify(notification);
    return newMeasurable;
  }

  /**
   * @param {string} id
   * @return {Promise<Models.Measurable>}
   */
  async archive(id) {
    let measurable = await models.Measurable.findById(id);
    return measurable.archive();
  }

  /**
   * @param {string} id
   * @return {Promise<Models.Measurable>}
   */
  async unArchive(id) {
    let measurable = await models.Measurable.findById(id);
    return measurable.unarchive();
  }

  /**
   * @param {string} id
   * @param {object} data
   * @param {object} user
   * @return {Promise<Models.Measurable>}
   */
  async updateOne(id, data, user) {
    let measurable = await models.Measurable.findById(id);
    let notification = await measurable.updateNotifications(user, data);
    notify(notification);
    return measurable.update(data);
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

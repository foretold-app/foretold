const _ = require('lodash');

const { notify } = require("../lib/notifications");

const { DataBase } = require('./data-base');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 */
class MeasurablesData extends DataBase {

  /**
   * @param data
   * @param {object} user
   * @return {Promise<*>}
   */
  async createOne(data, user) {
    const newMeasurable = await this.models.Measurable.create(data);
    let notification = await newMeasurable.creationNotification(user);
    notify(notification);
    return newMeasurable;
  }

  /**
   * @param {string} id
   * @return {Promise<Models.Measurable>}
   */
  async archive(id) {
    let measurable = await this.models.Measurable.findById(id);
    return measurable.archive();
  }

  /**
   * @param {string} id
   * @return {Promise<Models.Measurable>}
   */
  async unArchive(id) {
    let measurable = await this.models.Measurable.findById(id);
    return measurable.unarchive();
  }

  /**
   * @param {string} id
   * @param {object} data
   * @param {object} user
   * @return {Promise<Models.Measurable>}
   */
  async updateOne(id, data, user) {
    let measurable = await this.models.Measurable.findById(id);
    let notification = await measurable.updateNotifications(user, data);
    notify(notification);
    return measurable.update(data);
  }

  /**
   * @param {object} options
   * @param {string[]} options.states
   * @param {string} options.agentId
   * @return {Promise<*|Array<Model>>}
   */
  async getAll(options) {
    const { offset, limit, channelId, seriesId, creatorId } = options;

    let where = {
      state: {
        [this.models.sequelize.Op.ne]: "ARCHIVED"
      },
      [this.models.sequelize.Op.and]: [{
        channelId: {
          [this.models.sequelize.Op.in]: this.channelIdsLiteral(options.agentId),
        }
      }],
    };

    if (seriesId) {
      where.seriesId = { [this.models.sequelize.Op.eq]: seriesId };
    }
    if (creatorId) {
      where.creatorId = { [this.models.sequelize.Op.eq]: creatorId };
    }
    if (channelId) {
      where[this.models.sequelize.Op.and].push({ channelId });
    }

    const items = await this.models.Measurable.findAll({
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
    const restrictions = 'agentId' in options ? {
      channelId: {
        [this.models.sequelize.Op.in]: this.channelIdsLiteral(options.agentId)
      }
    } : {};
    return await this.models.Measurable.findOne({
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

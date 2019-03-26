const models = require("../models");
const { notify } = require("../lib/notifications");

class MeasurablesData {

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
        [models.sequelize.Op.ne]: "ARCHIVED"
      }
    };

    if (seriesId) {
      where.seriesId = { [models.sequelize.Op.eq]: seriesId };
    }
    if (creatorId) {
      where.creatorId = { [models.sequelize.Op.eq]: creatorId };
    }
    if (channelId) {
      where.channelId = { [models.sequelize.Op.eq]: channelId };
    }

    const items = await models.Measurable.findAll({
      limit: limit,
      offset: offset,
      order: [['createdAt', 'DESC']],
      where
    });

    return items;
  }

  /**
   * @param {string} id
   * @return {Promise<*>}
   */
  async getOne(id) {
    return await models.Measurable.findOne({where: {id}});
  }

}

module.exports = {
  MeasurablesData,
};

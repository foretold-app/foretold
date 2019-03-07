const Sequelize = require('sequelize');

const models = require("../models");
const { notify } = require("../lib/notifications");

const { UsersData } = require('./users-data');

class MeasurableData {

  constructor() {
    this.usersData = new UsersData();
  }

  /**
   * @param root
   * @param values
   * @param options
   * @return {Promise<*>}
   */
  async createMeasurable (root, values, options) {
    const {
      name,
      description,
      valueType,
      expectedResolutionDate,
      resolutionEndpoint,
      descriptionDate,
      descriptionEntity,
      descriptionProperty,
      channel
    } = values;
    let _auth0Id = await this.usersData.getAuth0Id(options);
    const user = await this.usersData.auth0User(_auth0Id);
    const newMeasurable = await models.Measurable.create({
      name,
      valueType,
      description,
      expectedResolutionDate,
      creatorId: user.agentId,
      descriptionEntity,
      descriptionDate,
      resolutionEndpoint,
      descriptionProperty,
      channel
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
    const _auth0Id = await this.usersData.getAuth0Id(options);
    const user = await this.usersData.auth0User(_auth0Id);
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
    let _auth0Id = await this.usersData.getAuth0Id(options);
    const user = await this.usersData.auth0User(_auth0Id);
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
    let _auth0Id = await this.usersData.getAuth0Id(options);
    const user = await this.usersData.auth0User(_auth0Id);
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
   * @param root
   * @param values
   * @param options
   * @return {Promise<*|Array<Model>>}
   */
  async findAll(root, values, options) {
    const { offset, limit, channel } = values;
    return await models.Measurable.findAll({
      limit: limit,
      offset: offset,
      order: [['createdAt', 'DESC']],
      where: {
        channel: {
          [Sequelize.Op.eq]: channel
        },
        state: {
          [Sequelize.Op.ne]: "ARCHIVED"
        }
      }
    });
  }

}

module.exports = {
  MeasurableData,
};

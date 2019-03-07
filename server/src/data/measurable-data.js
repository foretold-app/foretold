const models = require("../models");
const { notify } = require("../lib/notifications");
const { usersData } = require('./');

class MeasurableData {

  constructor() {

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
    let _auth0Id = await usersData.getAuth0Id(options);
    const user = await usersData.auth0User(_auth0Id);
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

}

module.exports = {
  MeasurableData,
};

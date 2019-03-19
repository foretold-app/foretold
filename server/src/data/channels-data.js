const models = require("../models");

class ChannelsData {

  constructor() {

  }

  async channelUpdate(root, values, options) {
    return true;
  }

  async channelCreate(root, values, options) {
    return true;
  }

  async AddPersonToChannel(root, values, options) {
    return true;
  }

  async RemovePersonFromChannel(root, values, options) {
    return true;
  }

}

module.exports = {
  ChannelsData,
};

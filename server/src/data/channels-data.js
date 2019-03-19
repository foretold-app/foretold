const models = require("../models");

class ChannelsData {

  constructor() {

  }

  async channelUpdate(root, values, options) {
    console.log(root, values, options);
    return true;
  }

  async channelCreate(root, values, options) {
    console.log(root, values, options);
    return true;
  }

  async addPersonToChannel(root, values, options) {
    console.log(root, values, options);
    return true;
  }

  async removePersonFromChannel(root, values, options) {
    console.log(root, values, options);
    return true;
  }

}

module.exports = {
  ChannelsData,
};

const _ = require('lodash');

const { API } = require('./api');
const { GlobalSettingsData } = require('../data/global-settings-data');

class Trigger {
  constructor(webhook) {
    this.webhook = webhook;
    this.api = new API();
    this.globalSetting = new GlobalSettingsData();
  }

  async main() {
    const merged = _.get(this.webhook, 'pull_request.merged');
    if (!merged) {
      console.log('PullRequest is merged');
    } else {
      console.log('PullRequest is not merged yet.');
    }

    const pullRequestNumber = _.get(this.webhook, 'number');
    if (!pullRequestNumber) {
      console.log('Pull Request number is required.');
      return false;
    }

    const dataJson = await this.api.getDataJson(pullRequestNumber);
    if (dataJson) {
      console.log('Data.json', dataJson);
      await this.globalSetting.updateEntityGraph(dataJson);
    } else {
      console.log(`Data.json file is not found in the PR ${pullRequestNumber}`);
    }

    return true;
  }
}

module.exports = {
  Trigger,
};

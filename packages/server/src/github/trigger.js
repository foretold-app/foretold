const assert = require('assert');
const _ = require('lodash');

const { API } = require('./api');
const { GlobalSettingsData } = require('../data/global-settings-data');

class Trigger {
  /**
   * @param {object} webhook
   */
  constructor(webhook) {
    this.webhook = webhook;
    this.api = new API();
    this.globalSetting = new GlobalSettingsData();

    assert(_.has(webhook, 'pull_request.merged'), 'WebHook is not in format #1');
    assert(_.has(webhook, 'number'), 'WebHook is not in format #2');
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    const merged = _.get(this.webhook, 'pull_request.merged');
    if (merged !== true) {
      console.log('PullRequest is not merged yet.');
      return false;
    }

    console.log('PullRequest is merged');

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

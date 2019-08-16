const assert = require('assert');
const _ = require('lodash');

const { GitHubApi } = require('./git-hub-api');
const { GlobalSettingsData } = require('../data/global-settings-data');

class Trigger {
  /**
   * @param {object} webhook
   * @param {string} xHubSignature
   */
  constructor(webhook, xHubSignature) {
    this.webhook = webhook;
    this.xHubSignature = xHubSignature;
    this.api = new GitHubApi();
    this.globalSetting = new GlobalSettingsData();

    assert(_.isObject(webhook), 'WebHook should be an object');
    assert(_.has(webhook, 'pull_request.merged'), 'WebHook is not in format #1');
    assert(_.has(webhook, 'number'), 'WebHook is not in format #2');
    assert(_.isString(xHubSignature), 'GitHub signature is required.');
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    const isTested = this.api.verifySignature(this.webhook, this.xHubSignature);
    if (isTested !== true) {
      console.log(`GitHub signature is not valid.`);
      return false;
    }

    const merged = _.get(this.webhook, 'pull_request.merged');
    if (merged !== true) {
      console.warn('PullRequest is not merged yet.');
      return false;
    } else {
      console.log('PullRequest is merged');
    }

    const pullRequestNumber = _.get(this.webhook, 'number');
    if (!pullRequestNumber) {
      console.warn('Pull Request number is required.');
      return false;
    }

    const dataJson = await this.api.getDataJson(pullRequestNumber);
    if (dataJson) {
      console.log('Data.json', dataJson);
      await this.globalSetting.updateEntityGraph(dataJson);
    } else {
      console.warn(`Data.json file is not found in the PR ${pullRequestNumber}`);
    }

    return true;
  }
}

module.exports = {
  Trigger,
};

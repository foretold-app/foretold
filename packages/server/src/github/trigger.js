const _ = require('lodash');

const { API } = require('./api');

class Trigger {
  constructor(webhook) {
    this.webhook = webhook;
    this.api = new API();
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

    const data = await this.api.getDataJson(pullRequestNumber);
    if (data) {
      console.log('Data.json', data);
    } else {
      console.log(`Data.json file is not found in the PR ${pullRequestNumber}`);
    }

    return true;
  }
}

module.exports = {
  Trigger,
};

const _ = require('lodash');

class Trigger {
  constructor(webhook) {
    this.webhook = webhook;
  }

  async main() {
    const merged = _.get(this.webhook, 'pull_request.merged');
    if (!merged) console.log('PullRequest is merged');
    else console.log('PullRequest is not merged yet.');
  }
}

module.exports = {
  Trigger,
};

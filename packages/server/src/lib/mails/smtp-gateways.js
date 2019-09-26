const _ = require('lodash');

class SmtpGateways {
  constructor(emailConfig) {
    this.gateways = emailConfig.gateways;
  }

  /**
   * @public
   * @return {object}
   */
  getDefault() {
    return _.find(this.gateways, 'default');
  }
}

module.exports = {
  SmtpGateways,
};

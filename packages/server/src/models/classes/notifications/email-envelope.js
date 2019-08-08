const assert = require('assert');
const _ = require('lodash');
const Mustache = require('mustache');

class EmailEnvelope {
  /**
   * @todo: use one file of two (notification, template)
   * @param {object} options
   * @param {string} options.subject
   * @param {string} options.body
   * @param {object} options.replacements
   * @param {string} options.to
   */
  constructor(options = {}) {
    assert(!!options.subject, 'Subject is required for Email Envelope.');
    assert(!!options.body, 'Body is required for Email Envelope.');

    this.subject = options.subject;
    this.body = options.body;
    this.replacements = options.replacements;
    this.to = options.to;
  }

  /**
   * @param {object} replacements
   * @return {EmailEnvelope}
   */
  instanceFactory(replacements) {
    const subject = Mustache.render(this.subject, replacements);
    const body = Mustache.render(this.body, replacements);

    return new EmailEnvelope({
      subject,
      body,
      replacements,
      to: this.to,
    });
  }
}

module.exports = {
  EmailEnvelope,
};

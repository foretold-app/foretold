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
    const subject = _.get(options, 'subject');
    const body = _.get(options, 'body');
    const replacements = _.get(options, 'replacements') || {};
    const to = _.get(options, 'to') || '';

    assert(!!subject, 'Subject is required.');
    assert(!!body, 'Body is required.');
    assert(_.isString(subject), 'Subject is not a string.');
    assert(_.isString(body), 'Body is not a string.');

    this.subject = subject;
    this.body = body;
    this.replacements = replacements;
    this.to = to;
  }

  /**
   * @public
   * @param {string | null} recipient
   * @returns {EmailEnvelope}
   */
  setRecipient(recipient) {
    this.to = recipient;
    return this;
  }

  /**
   * @public
   * @param {string} outerTemplate
   * @returns {EmailEnvelope}
   */
  setOuterTemplate(outerTemplate) {
    this.body = Mustache.render(outerTemplate, { innerTemplate: this.body });
    return this;
  }

  /**
   * @public
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

const assert = require('assert');
const _ = require('lodash');

const { EmailSender } = require('../lib/mails/email-sender');

class Mailer {
  constructor(envelope = {}) {
    assert(_.isObject(envelope), '"Envelope" should be an object.');
    assert(!!envelope.to, 'Email recipient is required.');
    assert(!!envelope.body, 'Email body is required.');
    assert(!!envelope.subject, 'Email subject is required.');
    assert(!!envelope.replacements, 'Email replacements is required.');

    this.envelope = envelope;

    this.to = envelope.to;
    this.body = envelope.body;
    this.subject = envelope.subject;
    this.replacements = envelope.replacements;
    this.authToken = envelope.authToken || '';
  }

  async main() {
    const mailHelper = new EmailSender({
      to: this.to,
      template: this.body,
      subject: this.subject,
      replacements: this.replacements,
      authToken: this.authToken,
    });
    await mailHelper.main();
    return true;
  }
}

module.exports = {
  Mailer,
};

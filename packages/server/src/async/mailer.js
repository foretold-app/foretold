const { MailHelper } = require('../mails/mail-helper');

class Mailer {
  constructor(envelope = {}) {
    this.envelope = envelope;

    this.to = envelope.to;
    this.body = envelope.body;
    this.subject = envelope.subject;
    this.replacements = envelope.replacements;
  }

  async main() {
    const mailHelper = new MailHelper({
      to: this.to,
      template: this.body,
      subject: this.subject,
      replacements: this.replacements,
    });
    await mailHelper.main();
    return true;
  }
}

module.exports = {
  Mailer,
};

class EmailEnvelopeTemplate {
  constructor(options = {}) {
    this.subject = options.subject;
    this.body = options.body;
  }

  toJSON() {
    return JSON.stringify({
      subject: this.subject,
      body: this.body,
    });
  }
}

module.exports = {
  EmailEnvelopeTemplate,
};

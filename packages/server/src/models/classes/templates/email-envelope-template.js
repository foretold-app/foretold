class EmailEnvelopeTemplate {
  constructor(options = {}) {
    this.subject = options.subject;
    this.body = options.body;
  }
}

module.exports = {
  EmailEnvelopeTemplate,
};

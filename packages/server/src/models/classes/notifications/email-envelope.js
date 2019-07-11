class EmailEnvelope {
  constructor(options = {}) {
    this.subject = options.subject;
    this.body = options.body;
  }
}

module.exports = {
  EmailEnvelope,
};

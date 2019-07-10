class EmailEnvelope {
  constructor(options = {}) {
    this.subject = options.subject;
    this.body = options.body;
    this.to = options.to;
    this.from = options.from;
  }
}

module.exports = {
  EmailEnvelope,
};

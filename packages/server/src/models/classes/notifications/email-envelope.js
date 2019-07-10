class EmailEnvelope {
  constructor(options = {}) {
    this.subject = options.subject;
    this.body = options.body;
    this.to = options.to;
    this.from = options.from;
  }

  toJSON() {
    return JSON.stringify({
      subject: this.subject,
      body: this.body,
      to: this.to,
      from: this.from,
    });
  }
}

module.exports = {
  EmailEnvelope,
};

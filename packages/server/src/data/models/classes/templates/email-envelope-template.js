class EmailEnvelopeTemplate {
  /**
   * @deprecated
   * @todo: use one file of two (notification, template)
   * @param {object} options
   * @param {string} options.subject
   * @param {string} options.body
   */
  constructor(options = {}) {
    this.subject = options.subject;
    this.body = options.body;
    // @todo: add assertions
  }
}

module.exports = {
  EmailEnvelopeTemplate,
};

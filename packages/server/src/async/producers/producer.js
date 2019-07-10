const data = require('../../data');

const { TEMPLATE_NAME } = require('../../models/enums/template-name');
const { EmailEnvelope } = require('../../models/classes/notifications');

class Producer {
  constructor(options = {}) {
    this.options = options;
    this.data = data;
    this.TEMPLATE_NAME = TEMPLATE_NAME;
    this.EmailEnvelope = EmailEnvelope;
    this.templateName = TEMPLATE_NAME.MEASURABLE_STATE_IS_CHANGED;
  }

  async getTemplate() {
    const params = { name: this.templateName };
    return this.data.templates.getOne(params);
  }
}

module.exports = {
  Producer,
};

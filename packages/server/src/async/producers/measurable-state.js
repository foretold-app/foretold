const assert = require('assert');
const _ = require('lodash');

const  { Producer } = require('./producer');

class MeasurableState extends Producer {
  constructor(measurable = {}) {
    super({});
    this.measurable = measurable;
  }

  async main() {
    try {
      const creator = await this.measurable.getCreator();
      assert(!!_.get(creator, 'id'), 'Creator ID is required');

      const template = await this._getTemplate();
      assert(!!_.get(template, 'id'), 'Template ID is required');
      assert(!!_.get(template, 'envelopeTemplate'), 'Envelope Template ID is required');

      const emailEnvelope = new this.EmailEnvelope(template.envelopeTemplate);
      const notification = await this._createEmailNotification(emailEnvelope);
      assert(!!_.get(notification, 'id'), 'Notification ID is required');

      const assignment = await this._assignNotification(creator.id, notification.id);
      assert(!!_.get(assignment, 'id'), 'Assignment ID is required');

    } catch (e) {
      console.log(`MeasurableState`, e.message, e);
    }
    return true;
  }
}

module.exports = {
  MeasurableState,
};

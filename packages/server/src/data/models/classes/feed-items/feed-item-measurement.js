const assert = require('assert');
const _ = require('lodash');
const Mustache = require('mustache');

const { FeedItemGeneric } = require('./feed-item-generic');
const { FEED_ITEM_BODY } = require('../../../../enums');

class FeedItemMeasurement extends FeedItemGeneric {
  /**
   * @public
   * @param {object} envelopeTemplate
   * @param {string} envelopeTemplate.item
   * @param {string} envelopeTemplate.description
   * @param {object} inputs
   * @param {Defs.MeasurableID} inputs.measurableId
   * @param {Defs.MeasurementID} inputs.measurementId
   */
  constructor(envelopeTemplate, inputs = {}) {
    super(envelopeTemplate, inputs);

    assert(_.isString(inputs.measurableId),
      'MeasurableId should be a string.');
    assert(_.isString(inputs.measurementId),
      'MeasurementId should be a string.');

    this.measurableId = inputs.measurableId;
    this.measurementId = inputs.measurementId;
  }

  /**
   * @public
   * @return {string}
   */
  getName() {
    return FEED_ITEM_BODY.measurement;
  }

  /**
   * @public
   * @param {object} replacements
   * @return {FeedItem}
   */
  instanceFactory(replacements) {
    const item = Mustache.render(this.item, replacements);
    const description = Mustache.render(this.description, replacements);

    return new FeedItemMeasurement({
      item,
      description,
    }, {
      measurableId: this.measurableId,
      measurementId: this.measurementId,
    });
  }
}

module.exports = {
  FeedItemMeasurement,
};

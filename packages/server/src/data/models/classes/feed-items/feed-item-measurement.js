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
   * @param {Defs.MeasurableID} envelopeTemplate.measurableId
   * @param {Defs.MeasurementID} envelopeTemplate.measurementId
   * @param {object} inputs
   */
  constructor(envelopeTemplate, inputs = {}) {
    super(envelopeTemplate, inputs);

    assert(_.isString(envelopeTemplate.measurableId),
      'MeasurableId should be a string.');
    assert(_.isString(envelopeTemplate.measurementId),
      'MeasurementId should be a string.');

    // @todo: It is collision with a template.
    this.measurableId = envelopeTemplate.measurableId;
    this.measurementId = envelopeTemplate.measurementId;
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
    const measurableId = Mustache.render(this.measurableId, replacements);
    const measurementId = Mustache.render(this.measurementId, replacements);

    return new FeedItemMeasurement({
      item,
      description,
      measurableId,
      measurementId,
    });
  }
}

module.exports = {
  FeedItemMeasurement,
};

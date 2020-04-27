const assert = require('assert');
const _ = require('lodash');
const Mustache = require('mustache');

const { FeedItemGeneric } = require('./feed-item-generic');
const { FEED_ITEM_BODY } = require('../../../../enums');

class FeedItemMeasurement extends FeedItemGeneric {
  /**
   * @public
   * @param {object} options
   * @param {string} options.item
   * @param {string} options.description
   * @param {Defs.MeasurableID} options.measurableId
   * @param {Defs.MeasurementID} options.measurementId
   */
  constructor(options) {
    super(options);
    assert(_.isString(options.measurableId),
      'MeasurableId should be a string.');
    assert(_.isString(options.measurementId),
      'MeasurementId should be a string.');
    this.measurableId = options.measurableId;
    this.measurementId = options.measurementId;
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
   * @return {string}
   */
  getMeasurableId() {
    return this.measurableId;
  }

  /**
   * @public
   * @return {string}
   */
  getMeasurementId() {
    return this.measurableId;
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

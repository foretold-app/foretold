const assert = require('assert');
const _ = require('lodash');
const Mustache = require('mustache');

const { FeedItemGeneric } = require('./feed-item-generic');
const { FEED_ITEM_BODY } = require('../../../../enums');

class FeedItemMeasurableB extends FeedItemGeneric {
  /**
   * @public
   * @param {object} options
   * @param {string} options.item
   * @param {string} options.description
   * @param {string} options.labelSubject
   * @param {string} options.labelProperty
   * @param {string} options.labelCustom
   * @param {string} options.labelStartAtDate
   * @param {string} options.labelEndAtDate
   * @param {string} options.labelConditionals
   * @param {Defs.MeasurableID} options.measurableId
   */
  constructor(options) {
    super(options);

    assert(!!_.has(options, 'labelSubject'),
      'labelSubject is required.');
    assert(!!_.has(options, 'labelProperty'),
      'labelProperty is required.');
    assert(!!_.has(options, 'labelCustom'),
      'labelCustom is required.');
    assert(!!_.has(options, 'labelStartAtDate'),
      'labelStartAtDate is required.');
    assert(!!_.has(options, 'labelEndAtDate'),
      'labelEndAtDate is required.');
    assert(!!_.has(options, 'labelConditionals'),
      'labelConditionals is required.');

    assert(_.isString(options.labelSubject),
      'labelSubject should be a string.');
    assert(_.isString(options.labelProperty),
      'labelProperty should be a string.');
    assert(_.isString(options.labelCustom),
      'labelCustom should be a string.');
    assert(_.isString(options.labelStartAtDate),
      'labelStartAtDate should be a string.');
    assert(_.isString(options.labelEndAtDate),
      'labelEndAtDate should be a string.');
    assert(_.isArray(options.labelConditionals),
      'labelConditionals should be an array.');

    assert(_.isString(options.measurableId),
      'MeasurableId should be a string.');
    this.measurableId = options.measurableId;
  }

  /**
   * @public
   * @return {string}
   */
  getName() {
    return FEED_ITEM_BODY.measurableB;
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

    const labelSubject = Mustache.render(this.labelSubject,
      replacements);
    const labelProperty = Mustache.render(this.labelProperty,
      replacements);
    const labelCustom = Mustache.render(this.labelCustom,
      replacements);
    const labelStartAtDate = Mustache.render(this.labelStartAtDate,
      replacements);
    const labelEndAtDate = Mustache.render(this.labelEndAtDate,
      replacements);
    const labelConditionals = Mustache.render(this.labelConditionals,
      replacements);

    return new FeedItemMeasurableB({
      item,
      description,
      measurableId,

      labelSubject,
      labelProperty,
      labelCustom,
      labelStartAtDate,
      labelEndAtDate,
      labelConditionals,
    });
  }
}

module.exports = {
  FeedItemMeasurableB,
};

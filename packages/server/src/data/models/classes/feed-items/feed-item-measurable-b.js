const assert = require('assert');
const _ = require('lodash');
const Mustache = require('mustache');

const { FeedItemGeneric } = require('./feed-item-generic');
const { FEED_ITEM_BODY } = require('../../../../enums');

class FeedItemMeasurableB extends FeedItemGeneric {
  /**
   * @public
   *
   * @param {object} envelopeTemplate
   * @param {string} envelopeTemplate.item
   * @param {string} envelopeTemplate.description
   *
   * @param {object} inputs
   * @param {string} inputs.labelSubject
   * @param {string} inputs.labelProperty
   * @param {string} inputs.labelCustom
   * @param {string} inputs.labelStartAtDate
   * @param {string} inputs.labelEndAtDate
   * @param {string[]} inputs.labelConditionals
   * @param {Defs.MeasurableID} inputs.measurableId
   */
  constructor(envelopeTemplate, inputs = {}) {
    super(envelopeTemplate, inputs);

    assert(_.isString(inputs.measurableId),
      'MeasurableId should be a string.');

    assert(!!_.has(inputs, 'labelSubject'),
      'labelSubject is required.');
    assert(!!_.has(inputs, 'labelProperty'),
      'labelProperty is required.');
    assert(!!_.has(inputs, 'labelCustom'),
      'labelCustom is required.');
    assert(!!_.has(inputs, 'labelStartAtDate'),
      'labelStartAtDate is required.');
    assert(!!_.has(inputs, 'labelEndAtDate'),
      'labelEndAtDate is required.');
    assert(!!_.has(inputs, 'labelConditionals'),
      'labelConditionals is required.');

    assert(
      _.isString(inputs.labelSubject)
      || _.isNull(inputs.labelSubject),
      'labelSubject should be a string.',
    );
    assert(
      _.isString(inputs.labelProperty)
      || _.isNull(inputs.labelProperty),
      'labelProperty should be a string.',
    );
    assert(
      _.isString(inputs.labelCustom)
      || _.isNull(inputs.labelCustom),
      'labelCustom should be a string.',
    );
    assert(
      _.isString(inputs.labelStartAtDate)
      || _.isNull(inputs.labelStartAtDate),
      'labelStartAtDate should be a string.',
    );
    assert(
      _.isString(inputs.labelEndAtDate)
      || _.isNull(inputs.labelEndAtDate),
      'labelEndAtDate should be a string.',
    );
    assert(
      _.isArray(inputs.labelConditionals)
      || _.isNull(inputs.labelConditionals),
      'labelConditionals should be an array.',
    );

    this.measurableId = inputs.measurableId;
    this.labelSubject = inputs.labelSubject;
    this.labelProperty = inputs.labelProperty;
    this.labelCustom = inputs.labelCustom;
    this.labelStartAtDate = inputs.labelStartAtDate;
    this.labelEndAtDate = inputs.labelEndAtDate;
    this.labelConditionals = inputs.labelConditionals;
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

    return new FeedItemMeasurableB({
      item,
      description,
    }, {
      measurableId: this.measurableId,
      labelSubject: this.labelSubject,
      labelProperty: this.labelProperty,
      labelCustom: this.labelCustom,
      labelStartAtDate: this.labelStartAtDate,
      labelEndAtDate: this.labelEndAtDate,
      labelConditionals: this.labelConditionals,
    });
  }
}

module.exports = {
  FeedItemMeasurableB,
};

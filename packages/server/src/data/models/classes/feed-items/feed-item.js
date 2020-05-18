const assert = require('assert');
const { AssertionError } = require('assert');
const _ = require('lodash');

/**
 * @abstract
 */
class FeedItem {
  /**
   * @param {object} envelopeTemplate
   * @param {object} inputs
   */
  constructor(envelopeTemplate, inputs = {}) {
    assert(_.isObject(envelopeTemplate),
      'envelopeTemplate should be an object.');
    assert(_.isObject(inputs),
      'inputs should be an object.');
  }

  /**
   * @public
   * @return {string}
   */
  getName() {
    throw AssertionError('Name is required.');
  }
}

module.exports = {
  FeedItem,
};

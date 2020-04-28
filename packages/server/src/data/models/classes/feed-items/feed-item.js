const assert = require('assert');
const { AssertionError } = require('assert');
const _ = require('lodash');

/**
 * @abstract
 */
class FeedItem {
  /**
   * @param {object} options
   */
  constructor(options) {
    assert(_.isObject(options), 'Options should be an object.');
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

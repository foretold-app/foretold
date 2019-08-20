const assert = require('assert');
const _ = require('lodash');

/**
 * @param {string} className
 * @param {object} options
 * @param {object | string[]} list
 * @return {string[]}
 */
module.exports.diff = function diff(className, options, list) {
  list = _.isArray(list) ? list : _.keys(list);
  const difference = _.difference(_.keys(options), list);
  if (difference.length > 0) {
    console.warn(`${className} doesn't expect "${difference.join(', ')}".`);
  }
  return difference;
};

/**
 * @param {string} className
 * @param {object} list
 * @param {object} context
 */
module.exports.test = function test(className, list, context) {
  _.each(list, (test, name) => {
    if (_.has(context, name)) {
      assert(
        test(context[name]),
        `${className}."${name}" assert failed, ` +
        `type is "${typeof _.get(context, name)}".`);
    }
  });
};

/**
 * @param {string} className
 * @param {object | string[]} list
 * @param {object} options
 * @param {object} context
 */
module.exports.extend = function extend2(className, options, list, context) {
  list = _.isArray(list) ? list : _.keys(list);
  _.each(list, (name) => {
    if (_.has(options, name)) {
      context[name] = _.get(options, name);
    }
  });
};

/**
 * @param {string} className
 * @param {object} options
 * @param {object} context
 */
module.exports.copy = function copy(className, options, context) {
  _.each(options, (value, name) => {
    context[name] = value;
  });
};


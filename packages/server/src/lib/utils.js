const assert = require('assert');
const _ = require('lodash');

/**
 * @param className
 * @param options
 * @param list
 * @return {string[]}
 */
module.exports.diff = function diff(className, options, list) {
  const difference = _.difference(_.keys(options), list);
  if (difference > 0) {
    console.warn(`${className} don't expect "${difference.join(', ')}".`);
  }
  return difference;
};

/**
 * @param className
 * @param options
 * @param list
 * @return {string[]}
 */
module.exports.diff2 = function diff2(className, options, list) {
  const difference = _.difference(_.keys(options), _.keys(list));
  if (difference > 0) {
    console.warn(`${className} don't expect "${difference.join(', ')}".`);
  }
  return difference;
};

/**
 * @param className
 * @param list
 * @param context
 */
module.exports.test2 = function test2(className, list, context) {
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
 * @param className
 * @param list
 * @param options
 * @param context
 */
module.exports.extend = function extend(className, options, list, context) {
  _.each(list, (name) => {
    if (_.has(options, name)) {
      context[name] = _.get(options, name);
    }
  });
};

/**
 * @param className
 * @param list
 * @param options
 * @param context
 */
module.exports.extend2 = function extend2(className, options, list, context) {
  _.each(list, (_test, name) => {
    if (_.has(options, name)) {
      context[name] = _.get(options, name);
    }
  });
};

/**
 * @param className
 * @param options
 * @param context
 */
module.exports.copy = function copy(className, options, context) {
  _.each(options, (value, name) => {
    context[name] = value;
  });
};


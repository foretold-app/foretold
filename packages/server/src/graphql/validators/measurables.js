const _ = require('lodash');

const { measurableEmptyName } = require('../../../config/lang');

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @return {Promise<boolean>}
 */
async function measurableNameValidation(root, args, _context, _info) {
  const name = _.get(args, 'input.name', null);
  const labelSubject = _.get(args, 'input.labelSubject', null);
  const isName = !!name || !!labelSubject;

  if (!isName) throw new Error(measurableEmptyName());

  return true;
}

module.exports = {
  measurableNameValidation,
};

const _ = require('lodash');

const lang = require('../../../config/lang');

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
  const labelProperty = _.get(args, 'input.labelProperty', null);

  if (!!name) {
    if (_.size(name) < 3) {
      throw new Error(lang.measurableNameLess3());
    }
  } else if (!!labelSubject && !!labelProperty) {
    if (_.size(labelSubject) < 3) {
      throw new Error(lang.measurableLabelSubjectLess3());
    }
    if (_.size(labelProperty) < 3) {
      throw new Error(lang.measurableLabelPropertyLess3());
    }
  } else {
    throw new Error(lang.measurableEmptyName());
  }

  return true;
}

module.exports = {
  measurableNameValidation,
};

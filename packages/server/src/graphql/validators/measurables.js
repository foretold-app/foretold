const _ = require('lodash');

const { KenFacade } = require('../../lib/ken-facade');
const lang = require('../../../config/lang');

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<boolean>}
 */
async function measurableNameValidation(root, args, context, _info) {
  const name = _.get(args, 'input.name', null);
  const labelSubject = _.get(args, 'input.labelSubject', null);
  const labelProperty = _.get(args, 'input.labelProperty', null);
  const entityGraph = _.get(context, 'globalSettings.entityGraph', {});

  const ken = new KenFacade(entityGraph);
  const kenNames = ken.names(labelSubject, labelProperty);

  if (!!name) {
    if (_.size(name) < 3) {
      throw new Error(lang.measurableNameLess3());
    }
  } else if (!!labelSubject && !!labelProperty) {
    // 1
    if (_.size(kenNames.subject) < 3) {
      throw new Error(lang.measurableLabelSubjectErr());
    }
    if (_.size(kenNames.subject) < 3) {
      throw new Error(lang.measurableLabelSubjectErr());
    }
    // 2, impenetrable
    if (_.size(kenNames.property) > 256) {
      throw new Error(lang.measurableLabelPropertyErr());
    }
    if (_.size(kenNames.property) > 256) {
      throw new Error(lang.measurableLabelPropertyErr());
    }
  } else {
    throw new Error(lang.measurableEmptyName());
  }

  return true;
}

module.exports = {
  measurableNameValidation,
};

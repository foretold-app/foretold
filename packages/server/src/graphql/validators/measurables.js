const _ = require('lodash');

const { KenFacade } = require('../../lib/ken-facade');
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
  if (!name) return true;
  if (_.size(name) < 3) throw new Error(lang.measurableNameLess());
  return true;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<boolean>}
 */
async function measurableLabelsValidation(root, args, context, _info) {
  const labelSubject = _.get(args, 'input.labelSubject', null);
  const labelProperty = _.get(args, 'input.labelProperty', null);
  const entityGraph = _.get(context, 'globalSettings.entityGraph', {});

  if (!labelSubject || !labelProperty) return true;

  const ken = new KenFacade(entityGraph);
  const kenNames = ken.names(labelSubject, labelProperty);

  // 1
  if (_.size(kenNames.subject) < 3) {
    throw new Error(lang.measurableLabelSubjectErr());
  }
  if (_.size(kenNames.property) < 3) {
    throw new Error(lang.measurableLabelPropertyErr());
  }

  // 2, impenetrable
  if (_.size(kenNames.subject) > 256) {
    throw new Error(lang.measurableLabelSubjectErr());
  }
  if (_.size(kenNames.property) > 256) {
    throw new Error(lang.measurableLabelPropertyErr());
  }

  return true;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @return {Promise<boolean>}
 */
async function measurableOnlyOneTitleValidation(root, args, _context, _info) {
  const name = _.get(args, 'input.name', null);
  const labelSubject = _.get(args, 'input.labelSubject', null);
  const labelProperty = _.get(args, 'input.labelProperty', null);

  if (!!name && (!!labelSubject || !!labelProperty)) {
    throw new Error(lang.measurableOnlyNameErr());
  } else if (!name && (!labelSubject || !labelProperty)) {
    throw new Error(lang.measurableEmptyName());
  }

  return true;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<boolean>}
 */
async function measurableConditionalOnValidation(root, args, context, _info) {
  const labelConditionals = _.get(args, 'input.labelConditionals', null);
  const entityGraph = _.get(context, 'globalSettings.entityGraph', {});

  if (!_.isArray(labelConditionals)) return true;
  if (labelConditionals.length === 0) return true;

  const ken = new KenFacade(entityGraph);

  for (let i = 0, max = labelConditionals.length; i < max; i++) {
    const labelConditional = labelConditionals[i];
    const name = ken.name(labelConditional);
    if (_.isEmpty(labelConditional)) {
      throw new Error(lang.conditionalOnItemEmpty());
    }
    if (_.isEmpty(name)) {
      throw new Error(lang.conditionalOnItemErr());
    }
    if (_.size(name) < 3) {
      throw new Error(lang.conditionalOnItemLess());
    }
    if (_.size(name) > 256) {
      throw new Error(lang.conditionalOnItemMore());
    }
  }

  return true;
}

module.exports = {
  measurableNameValidation,
  measurableLabelsValidation,
  measurableOnlyOneTitleValidation,
  measurableConditionalOnValidation,
};

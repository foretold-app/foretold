const _ = require('lodash');
const { rule } = require('graphql-shield');

/**
 * @param {object} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function isOwnerRule(root, args, context, info) {
  const creatorId = _.get(context, 'measurable.creatorId');
  const agentId = _.get(context, 'agent.id');
  const isOwner = creatorId === agentId;
  console.log(`\x1b[33m Rule Measurables (isOwner) "${isOwner}" \x1b[0m`);
  return isOwner;
}

/**
 * @param {object} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function isArchivedRule(root, args, context, info) {
  const isArchived = !!_.get(context, 'measurable.isArchived');
  console.log(`\x1b[33m Rule Measurables ` +
    `(isArchivedRule) "${isArchived}"\x1b[0m`);
  return isArchived;
}

/** @type {Rule} */
const isOwner = rule({ cache: 'no_cache' })(isOwnerRule);
/** @type {Rule} */
const isArchived = rule({ cache: 'no_cache' })(isArchivedRule);

module.exports = {
  isOwner,
  isArchived,

  isOwnerRule,
};

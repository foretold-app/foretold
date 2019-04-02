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
  const agentId = _.get(context, 'user.agentId');
  const isOwner = creatorId === agentId;
  console.log(`\x1b[33m Rule (isOwner) ${isOwner} \x1b[0m`);
  return isOwner;
}

/** @type {Rule} */
const isOwner = rule()(isOwnerRule);

module.exports = {
  isOwner,
};

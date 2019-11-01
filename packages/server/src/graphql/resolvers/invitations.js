const _ = require('lodash');

const data = require('../../data');
const { Data } = require('../../data/classes');

/**
 * @param {*} _root
 * @param {object} args
 * @param {object} args.input
 * @param {object} args.input.agentId
 * @param {object} args.input.channelId
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<boolean>}
 */
async function create(_root, args, context, _info) {
  const input = new Data({
    inviterAgentId: _.get(context, 'agent.id', null),
    email: _.get(args, 'input.email', null),
    channelId: _.get(args, 'input.channelId', null),
  });
  return data.invitations.invite(input);
}

module.exports = {
  create,
};

const _ = require('lodash');

const data = require('../../data');
const { Data } = require('../../data/classes/data');

/**
 * @param {*} root
 * @param {object} args
 * @param {object} args.input
 * @param {object} args.input.agentId
 * @param {object} args.input.channelId
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<boolean>}
 */
async function create(root, args, context, info) {
  const datas = new Data({
    inviterAgentId: _.get(context, 'agent.id'),
    email: _.get(args, 'input.email'),
    channelId: _.get(args, 'input.channelId'),
  });
  return data.invitations.invite(datas);
}

module.exports = {
  create,
};

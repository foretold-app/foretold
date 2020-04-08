const _ = require('lodash');

const { channelDoesNotExists } = require('../../../config/lang');
const { AgentData } = require('../../data');
const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Options } = require('../../data/classes');

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @return {Promise<boolean>}
 */
async function agentExistsValidation(root, args, _context, _info) {
  const agentId = _.get(args, 'agentId', null);

  const params = new Params({ id: agentId });
  const query = new Query();
  const options = new Options({ raw: true });
  const count = await new AgentData().getCount(params, query, options);

  if (!count) throw new Error(channelDoesNotExists());

  return true;
}

module.exports = {
  agentExistsValidation,
};

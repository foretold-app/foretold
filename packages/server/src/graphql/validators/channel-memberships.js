const _ = require('lodash');

const { agentIsNotInAChannel } = require('../../../config/lang');
const { ChannelMembershipsData } = require('../../data');
const logger = require('../../lib/log');

const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Options } = require('../../data/classes');

const log = logger.module('validators/channel-memberships');

/**
 * @todo: Should be divided on two middleware.
 * @todo: To fix "||" a joined logic.
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @return {Promise<boolean>}
 */
async function channelMembershipExistsValidation(root, args, _context, _info) {
  const channelId = _.get(args, 'input.channelId', null);
  const agentId = _.get(args, 'input.agentId', null);

  log.trace('\x1b[36m ---> \x1b[0m Validator '
    + '(agentIsInChannel)', { channelId, agentId });

  if (!channelId || !agentId) throw new Error(agentIsNotInAChannel());

  const params = new Params({ channelId, agentId });
  const query = new Query();
  const options = new Options({ raw: true });
  const count = await new ChannelMembershipsData().getCount(
    params, query, options,
  );

  if (!count) throw new Error(agentIsNotInAChannel());

  return true;
}

module.exports = {
  channelMembershipExistsValidation,
};

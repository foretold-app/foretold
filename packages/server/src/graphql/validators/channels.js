const _ = require('lodash');

const { channelDoesNotExists } = require('../../../config/lang');

const { ChannelsData } = require('../../data');

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
async function channelExistsValidation(root, args, _context, _info) {
  const channelId = _.get(args, 'channelId', null);

  const params = new Params({ id: channelId });
  const query = new Query();
  const options = new Options({ raw: true });
  const count = await new ChannelsData().getCount(params, query, options);

  if (!count) throw new Error(channelDoesNotExists());

  return true;
}

module.exports = {
  channelExistsValidation,
};

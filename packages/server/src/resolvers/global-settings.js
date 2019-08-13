const _ = require('lodash');

const data = require('../data');
const { Params } = require('../data/classes/params');
const { Options } = require('../data/classes/options');
const { Query } = require('../data/classes/query');

/**
 * @param {*} root
 * @param {object} args
 * @param {string} args.name
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Model>}
 */
async function one(root, args, context, info) {
  const name = _.get(args, 'name');

  const params = new Params({ name });
  const query = new Query();
  const options = new Options();

  return await data.globalSettings.getOne(
    params,
    query,
    options,
  );
}

module.exports = {
  one,
};

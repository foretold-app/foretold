const _ = require('lodash');

const { GlobalSettingsData } = require('../../data');

const { Params } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Data } = require('../../data/classes');

/**
 * @param {*} _root
 * @param {object} args
 * @param {string} args.name
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<Models.Model>}
 */
async function one(_root, args, _context, _info) {
  const name = _.get(args, 'name', null);

  const params = new Params({ name });
  const query = new Query();
  const options = new Options();

  return new GlobalSettingsData().getOne(
    params,
    query,
    options,
  );
}

/**
 * @param {*} _root
 * @param {object} args
 * @param {string} args.name
 * @param {Object} args.input
 * @param {object} args.input
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function update(_root, args, _context, _info) {
  const name = _.get(args, 'name', null);
  const input = _.get(args, 'input') || {};

  const params = new Params({ name });
  const datas = new Data(input);
  const options = new Options();

  return new GlobalSettingsData().updateOne(params, datas, options);
}

module.exports = {
  one,
  update,
};

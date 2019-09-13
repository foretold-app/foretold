const _ = require('lodash');

const data = require('../../data');

const { Params } = require('../../data/classes/params');
const { Options } = require('../../data/classes/options');
const { Query } = require('../../data/classes/query');
const { Data } = require('../../data/classes/data');

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

  return data.globalSettings.getOne(
    params,
    query,
    options,
  );
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.name
 * @param {Models.ObjectID} args.input
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function update(root, args, context, info) {
  const name = _.get(args, 'name');
  const input = _.get(args, 'input');

  const params = new Params({ name });
  const datas = new Data(input);
  const options = new Options();

  return data.globalSettings.updateOne(params, datas, options);
}

module.exports = {
  one,
  update,
};

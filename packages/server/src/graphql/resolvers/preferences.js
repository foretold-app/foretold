const _ = require('lodash');

const datas = require('../../data');

const { Options } = require('../../data/classes/options');
const { Params } = require('../../data/classes/params');
const { Query } = require('../../data/classes/query');
const { Data } = require('../../data/classes/data');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function getOne(root, args, context, info) {
  const agentId = _.get(root, 'id');
  const params = new Params({ agentId });
  const query = new Query();
  const data = new Data({ agentId });
  return datas.preferences.upsertOne(params, query, data);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.User>}
 */
async function update(root, args, context, info) {
  const params = new Params({ id: args.id });
  const data = new Data(args.input);
  const options = new Options();
  return datas.preferences.updateOne(params, data, options);
}

module.exports = {
  getOne,
  update,
};

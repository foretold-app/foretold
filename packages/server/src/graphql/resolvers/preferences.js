const _ = require('lodash');

const datas = require('../../data');

const { Options } = require('../../data/classes');
const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Data } = require('../../data/classes');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function getOne(root, args, context, info) {
  const agentId = _.get(root, 'id', null);
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
  const id = _.get(args, 'id', null);

  const params = new Params({ id });
  const data = new Data(args.input);
  const options = new Options();

  return datas.preferences.updateOne(params, data, options);
}

module.exports = {
  getOne,
  update,
};

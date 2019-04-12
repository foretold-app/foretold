const data = require('../data');

/**
 * @param {*} root
 * @param {object} args
 * @param {string} args.id
 * @param {object} args.input
 * @param {Schema.Context} options
 * @returns {Promise<Models.User>}
 */
async function create(root, args, options) {
  const datas = args.input;
  return await data.bots.createOne(datas);
}

module.exports = {
  create,
};

const data = require('../data');

/**
 * @param {*} root
 * @param {object} args
 * @param {string} args.id
 * @param {object} args.input
 * @param {Schema.Context} options
 * @returns {Promise<Models.User>}
 */
async function update(root, args, options) {
  const { id } = args;
  const datas = args.input;
  // @todo: user!
  const user = options.user;
  return await data.users.updateOne(id, datas, user);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} options
 * @returns {Promise<Models.User>}
 */
async function one(root, args, options) {
  const { id, auth0Id } = args;
  if (options.user) {
    return options.user;
  } else if (id) {
    return await data.users.getOne({ id });
  } else if (auth0Id) {
    return await data.users.getUserByAuth0Id(auth0Id);
  }
}

module.exports = {
  one,
  update,
};

const data = require('../data');

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {object} args.input
 * @param {Schema.Context} context
 * @returns {Promise<Models.User>}
 */
async function update(root, args, context) {
  const { id } = args;
  const datas = args.input;
  const user = context.user;
  return data.users.updateOne(id, datas, user);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @returns {Promise<Models.User>}
 */
async function one(root, args, context) {
  const { id, auth0Id } = args;
  if (context.user) {
    return context.user;
  } else if (id) {
    return data.users.getOne({ id });
  } else if (auth0Id) {
    return data.users.getUserByAuth0Id(auth0Id);
  }
}

module.exports = {
  one,
  update,
};

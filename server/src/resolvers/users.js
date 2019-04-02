const data = require('../data');

/**
 * @param root
 * @param args
 * @param options
 * @returns {Promise<Models.User>}
 */
async function update(root, args, options) {
  const { id, name } = args;
  const datas = { name };
  const user = options.user;
  return await data.usersData.updateOne(id, datas, user);
}

/**
 * @param root
 * @param args
 * @param options
 * @returns {Promise<Models.User>}
 */
async function one(root, args, options) {
  const { id, auth0Id } = args;
  if (options.user) {
    return options.user;
  } else if (id) {
    return await data.usersData.getOne({ id });
  } else if (auth0Id) {
    return await data.usersData.getUserByAuth0Id(auth0Id);
  }
}

module.exports = {
  one,
  update,
};

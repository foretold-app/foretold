const data = require('../data');

/**
 * @param root
 * @param args
 * @param options
 * @returns {Promise<Model>}
 */
async function update(root, args, options) {
  const { id, name } = args;
  const datas = { name };
  const user = options.user;
  return await data.usersData.updateOne(id, datas, user);
}

module.exports = {
  update,
};

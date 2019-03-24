const data = require('../data');

/**
 * @param root
 * @param values
 * @param options
 * @returns {Promise<Model>}
 */
async function edit(root, values, options) {
  return await data.usersData.edit(root, values, options);
}

module.exports = {
  edit,
};

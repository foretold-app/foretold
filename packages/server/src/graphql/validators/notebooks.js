const _ = require('lodash');

const { notebookDoesNotExists } = require('../../../config/lang');
const { NotebooksData } = require('../../data');
const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Options } = require('../../data/classes');

/**
 * @param {*} root
 * @param {object} args
 * @param {Defs.NotebookID} args.notebookId
 * @param {Schema.Context} _context
 * @param {object} _info
 * @return {Promise<boolean>}
 */
async function notebookExistsValidation(root, args, _context, _info) {
  const notebookId = _.get(args, 'notebookId', null);

  const params = new Params({ id: notebookId });
  const query = new Query();
  const options = new Options({ raw: true });
  const count = await new NotebooksData().getCount(params, query, options);

  if (!count) throw new Error(notebookDoesNotExists());

  return true;
}

module.exports = {
  notebookExistsValidation,
};

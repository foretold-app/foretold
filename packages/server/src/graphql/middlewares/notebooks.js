const _ = require('lodash');

const { notebookDoesNotExists } = require('../../../config/lang');

const { NotebooksData } = require('../../data');
const logger = require('../../lib/log');

const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Options } = require('../../data/classes');

const log = logger.module('middlewares/notebooks');

/**
 * @param {*} root
 * @param {object} args
 * @param {Definitions.NotebookID} args.notebookId
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

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Definitions.NotebookID} args.notebookId
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextNotebook(root, args, context, _info) {
  const notebookId = _.get(args, 'id', null);

  log.trace(
    '\x1b[36m ---> \x1b[0m Middleware (setContextNotebook)',
    { notebookId },
  );

  if (!!notebookId) {
    const params = new Params({ id: notebookId });
    const query = new Query();
    const options = new Options({ raw: true });
    context.notebook = await new NotebooksData().getOne(params, query, options);
  } else {
    context.notebook = null;
  }
}

module.exports = {
  notebookExistsValidation,

  setContextNotebook,
};

const _ = require('lodash');

const { NotebooksData } = require('../../data');
const logger = require('../../lib/log');

const { Params } = require('../../data/classes');

const log = logger.module('middlewares/notebooks');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.NotebookID} args.notebookId
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
    context.notebook = await new NotebooksData().getOne(params);
  } else {
    context.notebook = null;
  }
}

module.exports = {
  setContextNotebook,
};

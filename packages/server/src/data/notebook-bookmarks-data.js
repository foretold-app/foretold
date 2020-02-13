const { DataBase } = require('./data-base');
const { NotebookBookmarkModel } = require('../models');

const { Options } = require('../data/classes');
const { Params } = require('../data/classes');
const { Data } = require('../data/classes');
const { Query } = require('../data/classes');

const logger = require('../lib/log');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {VoteModel} model
 */
class NotebookBookmarksData extends DataBase {
  constructor() {
    super();
    this.model = new NotebookBookmarkModel();
    this.log = logger.module('NotebookBookmarksData');
  }

  /**
   * @param {Models.NotebookID} notebookId
   * @param {Models.AgentID} agentId
   * @returns {Promise<boolean>}
   */
  async toggle(notebookId, agentId) {
    const notebookBookmark = await this.one(notebookId, agentId);
    if (!!notebookBookmark) {
      await this.delete(notebookId, agentId);
      return false;
    }
    await this.create(notebookId, agentId);
    return true;
  }

  /**
   * @param {Models.NotebookID} notebookId
   * @param {Models.AgentID} agentId
   * @returns {Promise<boolean>}
   */
  async create(notebookId, agentId) {
    const data = new Data({ notebookId, agentId });
    const options = new Options({ raw: true });
    return this.createOne(data, options);
  }

  /**
   * @param {Models.NotebookID} notebookId
   * @param {Models.AgentID} agentId
   * @returns {Promise<boolean>}
   */
  async one(notebookId, agentId) {
    const params = new Params({ notebookId, agentId });
    const query = new Query();
    const options = new Options({ raw: true });
    return this.getOne(params, query, options);
  }

  /**
   * @param {Models.NotebookID} notebookId
   * @param {Models.AgentID} agentId
   * @returns {Promise<boolean>}
   */
  async delete(notebookId, agentId) {
    const params = new Params({ notebookId, agentId });
    const query = new Query();
    return this.deleteOne(params, query);
  }
}

module.exports = {
  NotebookBookmarksData,
};

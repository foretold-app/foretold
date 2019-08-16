const assert = require('assert');
const _ = require('lodash');
const request = require('request');
const crypto = require('crypto');

const config = require('../config');

class API {
  /**
   * @public
   */
  constructor() {
    this.repoOwner = config.GITHUB_REPO_OWNER;
    this.repoName = config.GITHUB_REPO_NAME;
    this.token = config.GITHUB_PERSONAL_ACCESS_TOKEN;
    this.webhookSecret = config.GITHUB_WEBHOOK_SECRET;
    this.serverURL = config.SERVER_URL;
    this.apiURL = config.GITHUB_API_URL;
    this.userAgent = config.GITHUB_QUERY_USER_AGENT;

    this.isReady =
      !!this.repoOwner && !!this.repoName &&
      !!this.token && !!this.webhookSecret;

    this.hookUrl = `${this.serverURL}/hooks`;
    this.gitHubRepoUrl =
      `${this.apiURL}/repos/${this.repoOwner}/${this.repoName}`;
    this.gitHubHooksUrl = `${this.gitHubRepoUrl}/hooks`;

    if (!this.repoOwner) console.warn(`GitHub repo owner is not set.`);
    if (!this.repoName) console.warn(`GitHub repo name is not set.`);
    if (!this.token) console.warn(`GitHub personal access token is not set, ` +
      `see https://github.com/settings/tokens.`);
    if (!this.webhookSecret) console.warn(`GitHub webhook secret is not set`);
  }

  /**
   * @public
   * @return {Promise<boolean|Object>}
   */
  async addHook() {
    await this._checkIfAllIsReady();

    if (await this._checkUrl() !== null) {
      console.warn(`GitHub web hook is already added.`);
      return false;
    }

    const hook = {
      name: 'web',
      active: true,
      events: ['pull_request'],
      config: {
        url: this.hookUrl,
        secret: this.webhookSecret,
        content_type: 'json',
        insecure_ssl: '1',
      },
    };
    const url = this.gitHubHooksUrl;
    return this._query(url, 'POST', hook);
  }

  /**
   * @public
   * @param {number} pullRequestNumber
   * @return {Promise<Object|boolean>}
   */
  async getDataJson(pullRequestNumber) {
    await this._checkIfAllIsReady();

    const files = await this._getPullFiles(pullRequestNumber);
    const file = _.find(files, ['filename', 'data.json'])
      || _.find(files, ['filename', 'Data.json']);
    if (!file) {
      console.warn('GitHub data.json file is not found');
      return false;
    }

    const contents_url = _.get(file, 'contents_url');
    console.log('GitHub contents_url', contents_url);
    const contents = await this._query(contents_url);
    if (!contents) {
      console.warn('GitHub data.json content file is not found');
      return false;
    }

    const download_url = _.get(contents, 'download_url');
    console.log('GitHub download_url', download_url);
    if (!download_url) {
      console.warn('GitHub download url is not found.');
      return false;
    }
    return await this._query(download_url);
  }

  /**
   * @public
   * @param {object} payload of webhook
   * @param {string} comparedHash
   * @return {boolean}
   */
  verifySignature(payload, comparedHash) {
    const payloadAsStr = JSON.stringify(payload);
    const hash = crypto
      .createHmac('sha1', config.GITHUB_WEBHOOK_SECRET)
      .update(payloadAsStr)
      .digest('hex');
    const hasWithPrefix = `sha1=${hash}`;
    return hasWithPrefix === comparedHash;
  }

  /**
   * @protected
   * @param {string} uri
   * @param {string} method
   * @param {null | object} body
   * @return {Promise<object>}
   */
  async _query(uri, method = 'GET', body = null) {
    assert(_.isString(uri), 'URI should be a string');
    assert(_.isString(method), 'Method should be a string');
    assert(_.isObject(body) || _.isNull(body), 'Method should be a string');

    const options = {
      uri,
      method,
      body,
      headers: this._getHeaders(),
      json: true,
      followAllRedirects: true
    };
    // console.log('GitHut query options', options);
    return new Promise((resolve, reject) => {
      request(options, (error, response, body) => {
        if (error) return reject(error);
        resolve(body)
      });
    });
  }

  /**
   * @protected
   * @return {{Authorization: string, Accept: string, "User-Agent": string}}
   */
  _getHeaders() {
    assert(_.isString(this.token),
      'GitHub personal access token should be a string');
    return {
      'Authorization': `bearer ${this.token}`,
      'User-Agent': this.userAgent,
      'Accept': 'application/vnd.github.v3+json',
    };
  }

  /**
   * @protected
   * @param {number} pullRequestNumber
   * @return {string}
   */
  _getPullFilesUrl(pullRequestNumber) {
    return `${this.gitHubRepoUrl}/pulls/${pullRequestNumber}/files`
  }

  /**
   * @protected
   * @return {Promise<Object>}
   */
  async getListOfHooks() {
    const url = this.gitHubHooksUrl;
    return this._query(url);
  }

  /**
   * @protected
   * @return {Promise<object>}
   */
  async _checkUrl() {
    const listHooks = await this.getListOfHooks();
    return _.find(listHooks, ['config.url', this.hookUrl]);
  }

  /**
   * @protected
   * @param {number} pullRequestNumber
   * @return {Promise<Object[]>}
   */
  async _getPullFiles(pullRequestNumber) {
    const url = this._getPullFilesUrl(pullRequestNumber);
    return this._query(url);
  }

  /**
   * @return {Promise<boolean>}
   * @protected
   */
  async _checkIfAllIsReady() {
    if (this.isReady === false) {
      throw new Error(`GitHub integration is turned off`);
    }
    return true;
  }

}

module.exports = {
  API,
};

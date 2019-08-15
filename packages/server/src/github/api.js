const assert = require('assert');
const _ = require('lodash');
const request = require('request');

const config = require('../config');

class API {

  constructor() {
    this.repoOwner = config.GITHUB_REPO_OWNER;
    this.repoName = config.GITHUB_REPO_NAME;
    this.token = config.GITHUB_PERSONAL_ACCESS_TOKEN;
    this.webhookSecret = config.GITHUB_WEBHOOK_SECRET;
    this.apiURL = `https://api.github.com`;
    this.serverURL = config.SERVER_URL;
    this.hookUrl = `${this.serverURL}/hooks`;
    this.isReady = true;

    if (!this.repoOwner) {
      console.warn(`GitHub repo owner is not set.`);
      this.isReady = false;
    }
    if (!this.repoName) {
      console.warn(`GitHub repo name is not set.`);
      this.isReady = false;
    }
    if (!this.token) {
      console.warn(
        `GitHub personal access token is not set, ` +
        `see https://github.com/settings/tokens.`
      );
      this.isReady = false;
    }
    if (!this.webhookSecret) {
      console.warn(`GitHub webhook secret is not set`);
      this.isReady = false;
    }
  }

  /**
   * @param {string} uri
   * @param {string} method
   * @param {null | object} body
   * @return {Promise<object>}
   */
  async query(uri, method = 'GET', body = null) {
    assert(_.isString(uri), 'URI should be a string');
    assert(_.isString(method), 'Method should be a string');
    assert(_.isObject(body) || _.isNull(body), 'Method should be a string');

    const options = {
      uri,
      method,
      body,
      headers: this.getHeaders(),
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
   * @return {{Authorization: string, Accept: string, "User-Agent": string}}
   */
  getHeaders() {
    assert(_.isString(this.token),
      'GitHub personal access token should be a string');
    return {
      'Authorization': `bearer ${this.token}`,
      'User-Agent': 'Foretold App',
      'Accept': 'application/vnd.github.v3+json',
    };
  }

  /**
   * @param {number} pullRequestNumber
   * @return {string}
   */
  getPullFilesUrl(pullRequestNumber) {
    return `${this.getRepo()}/pulls/${pullRequestNumber}/files`
  }

  /**
   * @return {string}
   */
  getHooks() {
    return `${this.getRepo()}/hooks`;
  }

  /**
   * @return {string}
   */
  getRepo() {
    return `${this.apiURL}/repos/${this.repoOwner}/${this.repoName}`;
  }

  /**
   * @return {Promise<Object>}
   */
  async getListOfHooks() {
    const url = this.getHooks();
    return this.query(url);
  }

  /**
   * @public
   * @return {Promise<boolean|Object>}
   */
  async addHook() {
    await this.checkIfAllIsReady();

    if (await this.checkUrl() !== null) {
      console.log(`GitHub web hook is already added.`);
      return false;
    }

    const hook = {
      "name": "web",
      "active": true,
      "events": [
        "pull_request"
      ],
      "config": {
        "url": this.hookUrl,
        "secret": this.webhookSecret,
        "content_type": "json",
        "insecure_ssl": "1"
      }
    };
    const url = this.getHooks();
    return this.query(url, 'POST', hook);
  }

  /**
   * @return {Promise<object>}
   */
  async checkUrl() {
    const listHooks = await this.getListOfHooks();
    return _.find(listHooks, ['config.url', this.hookUrl]);
  }

  /**
   * @param {number} pullRequestNumber
   * @return {Promise<Object[]>}
   */
  async getPullFiles(pullRequestNumber) {
    const url = this.getPullFilesUrl(pullRequestNumber);
    return this.query(url);
  }

  /**
   * @public
   * @param {number} pullRequestNumber
   * @return {Promise<Object|boolean>}
   */
  async getDataJson(pullRequestNumber) {
    await this.checkIfAllIsReady();

    const files = await this.getPullFiles(pullRequestNumber);
    const file = _.find(files, ['filename', 'data.json'])
      || _.find(files, ['filename', 'Data.json']);
    if (!file) {
      console.log('GitHub data.json file is not found');
      return false;
    }

    const contents_url = _.get(file, 'contents_url');
    console.log('GitHub contents_url', contents_url);
    const contents = await this.query(contents_url);
    if (!contents) {
      console.log('GitHub data.json content file is not found');
      return false;
    }

    const download_url = _.get(contents, 'download_url');
    console.log('GitHub download_url', download_url);
    if (!download_url) {
      console.log('GitHub download url is not found.');
      return false;
    }
    return await this.query(download_url);
  }

  async checkIfAllIsReady() {
    if (this.isReady === false) {
      throw new Error(`GitHub integration is turned off`);
    }
    return true;
  }
}

module.exports = {
  API,
};

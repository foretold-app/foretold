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

    if (!this.repoOwner) console.warn(`GitHub repo owner is not set.`);
    if (!this.repoName) console.warn(`GitHub repo name is not set.`);
    if (!this.token) console.warn(
      `GitHub personal access token is not set, ` +
      `see https://github.com/settings/tokens.`
    );
    if (!this.webhookSecret) console.warn(`GitHub webhook secret is not set`);
  }

  async query(uri, method = 'GET', body = null) {
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

  getHeaders() {
    return {
      'Authorization': `bearer ${this.token}`,
      'User-Agent': 'Foretold App',
      'Accept': 'application/vnd.github.v3+json',
    };
  }

  getPullFilesUrl(pullRequestNumber) {
    return `${this.getRepo()}/pulls/${pullRequestNumber}/files`
  }

  getHooks() {
    return `${this.getRepo()}/hooks`;
  }

  getRepo() {
    return `${this.apiURL}/repos/${this.repoOwner}/${this.repoName}`;
  }

  async getListOfHooks() {
    const url = this.getHooks();
    return this.query(url);
  }

  async addHook() {
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

  async checkUrl() {
    const listHooks = await this.getListOfHooks();
    return _.find(listHooks, ['config.url', this.hookUrl]);
  }

  async getPullFiles(pullRequestNumber) {
    const url = this.getPullFilesUrl(pullRequestNumber);
    return this.query(url);
  }

  async getDataJson(pullRequestNumber = 7) {
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
    console.log('GitHub download_url', contents_url);
    if (!download_url) {
      console.log('GitHub download url is not found.');
      return false;
    }
    return await this.query(download_url);
  }
}

const api = new API();

module.exports = {
  api,
  API,
};

const request = require('request');

const config = require('../config');

class API {

  constructor(repoOwner, repoName, token, webhookSecret) {
    this.repoOwner = repoOwner;
    this.repoName = repoName;
    this.token = token;
    this.webhookSecret = webhookSecret;
    this.apiURL = `https://api.github.com`;

    if (!repoOwner) console.warn(`GitHub repo owner is not set.`);
    if (!repoName) console.warn(`GitHub repo name is not set.`);
    if (!token) console.warn(
      `GitHub personal access token is not set, ` +
      `see https://github.com/settings/tokens.`
    );
    if (!webhookSecret) console.warn(`GitHub webhook secret is not set`);
  }

  async query(uri, method = 'GET', body = null) {
    const options = {
      uri,
      method,
      body,
      headers: this.getHeaders(),
      json: true
    };
    console.log('GitHut query options', options);
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
    const hook = {
      "name": "web",
      "active": true,
      "events": [
        "pull_request"
      ],
      "config": {
        "url": "http://dev.wirnex.com:31000/hooks",
        "secret": this.webhookSecret,
        "content_type": "json",
        "insecure_ssl": "1"
      }
    };
    const url = this.getHooks();
    return this.query(url, 'POST', hook);
  }
}

const api = new API(
  config.GITHUB_REPO_OWNER,
  config.GITHUB_REPO_NAME,
  config.GITHUB_PERSONAL_ACCESS_TOKEN,
  config.GITHUB_WEBHOOK_SECRET,
);

module.exports = {
  api,
  API,
};

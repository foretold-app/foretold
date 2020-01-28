const _ = require('lodash');
const assert = require('assert');
const graphqlClient = require('graphql-client');

const queries = require('./interfaces');

class API {
  /**
   * @param {string | null} API_URL
   * @param {string | null} token
   */
  constructor(API_URL, token) {
    this.API_URL = API_URL;
    this.token = token;
    this.graphqlClient = graphqlClient;
    this.queries = queries;
  }

  /**
   * @param token
   */
  setToken(token) {
    this.token = token;
  }

  /**
   * @public
   * @return {*}
   */
  async authentication(auth0jwt, auth0accessToken) {
    assert(_.isString(auth0jwt), 'auth0jwt is required.');
    assert(_.isString(auth0accessToken), 'auth0accessToken is required.');
    const result = await this._query(this.queries.authentication, {
      auth0jwt,
      auth0accessToken,
    });
    return this._getEntity('authentication')(result);
  }

  /**
   * @public
   * @return {*}
   */
  async measurements(params) {
    const result = await this._query(this.queries.measurements, params);
    return this._getList('measurements')(result);
  }

  /**
   * @public
   * @return {*}
   */
  async measurables(params) {
    const result = await this._query(this.queries.measurables, params);
    return this._getList('measurables')(result);
  }

  /**
   * @public
   * @return {object}
   */
  async authenticated() {
    const result = await this._query(this.queries.authenticated);
    return this._getEntity('authenticated')(result);
  }

  /**
   * @public
   * @return {object}
   */
  async channelCreate(input) {
    const result = await this._query(this.queries.channelCreate, { input });
    return this._getEntity('channelCreate')(result);
  }

  /**
   * @public
   * @return {object}
   */
  async measurableCreate(input) {
    const result = await this._query(this.queries.measurableCreate, { input });
    return this._getEntity('measurableCreate')(result);
  }

  /**
   * @public
   * @return {object}
   */
  async measurementCreate(input) {
    const result = await this._query(this.queries.measurementCreate, { input });
    return this._getEntity('measurementCreate')(result);
  }

  /**
   * @public
   * @return {object}
   */
  async channel(id) {
    const result = await this._query(this.queries.channel, { id });
    return this._getEntity('channel')(result);
  }

  /**
   * @protected
   * @return {{query}}
   */
  _getClient() {
    const config = {
      url: this.API_URL,
      headers: {},
    };
    if (!!this.token) {
      config.headers.Authorization = 'Bearer ' + this.token;
    }
    return this.graphqlClient(config);
  }

  /**
   * @protected
   * @param query
   * @param {object} [variables]
   * @return {*}
   */
  _query(query, variables = {}) {
    return this._getClient().query(query, variables, (req, res) => {
      if (res.status === 401) {
        throw new Error('Not authorized');
      }
    }).then((body) => {
      // console.log(query, variables, body);
      return body;
    }).catch((err) => {
      console.log('Query Error', err.message);
      return Promise.reject(err);
    });
  }

  /**
   * @protected
   * @param {string} alias
   * @return {Function}
   */
  _getList(alias) {
    return (result) => {
      this._proceedErrors(result);
      const edges = _.get(result, ['data', alias, 'edges'], []);
      return edges.map(edge => edge.node);
    };
  }

  /**
   * @protected
   * @param {string} alias
   * @return {Function}
   */
  _getEntity(alias) {
    return (result) => {
      this._proceedErrors(result);
      return _.get(result, ['data', alias]);
    };
  }

  /**
   * @param result
   */
  _proceedErrors(result) {
    const errors = _.get(result, 'errors', []);
    if (errors.length === 0) return;
    _.each(errors, (err) => {
      const message = _.get(err, 'message');
      console.error(message);
    });
  }
}

module.exports = {
  API,
};

const _ = require('lodash');
const assert = require('assert');
const graphqlClient = require('graphql-client');

const queries = require('./interfaces');

class API {
  /**
   * @param {string} token
   */
  constructor(token) {
    this.token = "";
    this.graphqlClient = graphqlClient;
    this.queries = queries;

    this.client = this.getClient();
  }

  /**
   * @protected
   * @return {{query}}
   */
  getClient() {
    return this.graphqlClient({
      url: this.config.API_URL,
      // headers: {
      //   Authorization: 'Bearer ' + this.token,
      // }
    });
  }

  /**
   * @protected
   * @param query
   * @param {object} [variables]
   * @return {*}
   */
  query(query, variables = {}) {
    return this.client.query(query, variables, (req, res) => {
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
   * @public
   * @return {*}
   */
  async measurables() {
    return this.query(this.queries.measurables);
  }


  /**
   * @public
   * @return {*}
   */
  async mutexTake(name = 'aggregation') {
    assert(_.isString(name), 'Name for "mutexTake" is required.');
    const result = await this.query(this.queries.mutexTake, { name });
    return this.getEntity('mutexTake')(result);
  }

  /**
   * @public
   * @return {*}
   */
  async measurements(params) {
    const result = await this.query(this.queries.measurements, params);
    return this.getList('measurements')(result);
  }

  /**
   * @public
   * @return {object}
   */
  async queryAuthenticated() {
    const result = await this.query(this.queries.authenticated);
    return this.getEntity('authenticated')(result);
  }

  /**
   * @protected
   * @param {string} alias
   * @return {Function}
   */
  getList(alias) {
    return (result) => {
      this.proceedErrors(result);
      const edges = _.get(result, ['data', alias, 'edges'], []);
      return edges.map(edge => edge.node);
    };
  }

  /**
   * @protected
   * @param {string} alias
   * @return {Function}
   */
  getEntity(alias) {
    return (result) => {
      this.proceedErrors(result);
      return _.get(result, ['data', alias]);
    };
  }

  /**
   * @param result
   */
  proceedErrors(result) {
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

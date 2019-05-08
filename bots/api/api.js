const _ = require('lodash');
const graphqlClient = require('graphql-client');

const queries = require('./queries');
const config = require('../config');

class API {
  /**
   * @param {string} token
   */
  constructor(token) {
    this.token = token;
    this.graphqlClient = graphqlClient;
    this.queries = queries;
    this.config = config;

    this.client = this.getClient();
  }

  /**
   * @protected
   * @return {{query}}
   */
  getClient() {
    return this.graphqlClient({
      url: this.config.API_URL,
      headers: {
        Authorization: 'Bearer ' + this.token,
      }
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
      // console.log(body);
      return body;
    }).catch((err) => {
      console.log(err.message);
      return Promise.reject(err);
    });
  }

  /**
   * @todo: use streams
   * @public
   * @return {*}
   */
  async measurables() {
    await this.query(this.queries.measurables);
  }

  /**
   * @public
   * @return {*}
   */
  async measurementCreate({ floatCdf, ...rest}) {
    const result = await this.query(this.queries.measurementCreate, {
      input: {
        value: { floatCdf },
        ...rest,
      },
    });
    return this.getList('measurementCreate')(result);
  }

  /**
   * @public
   * @return {*}
   */
  measurementCreateAggregation(params) {
    return this.measurementCreate({
      competitorType: 'AGGREGATION',
      ...params,
    });
  }

  /**
   * @todo: use streams
   * @public
   * @return {*}
   */
  async measurements(params) {
    const result = await this.query(this.queries.measurements, params);
    return this.getList('measurements')(result);
  }

  /**
   * @public
   * @return {*}
   */
  async measurementsCompetitive(params) {
    return this.measurements({
      competitorType: ['COMPETITIVE'],
      ...params,
    });
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
    }
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

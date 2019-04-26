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
      console.log(body);
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
  async measurables({ aggregatedBefore }) {
    const result = await this.query(this.queries.measurables, {
      aggregatedBefore,
    });
    return this.getList('measurables')(result);
  }

  /**
   * @public
   * @return {*}
   */
  async measurablesForAggregation() {
    return this.measurables({
      aggregatedBefore: new Date(),
    });
  }

  /**
   * @public
   * @return {*}
   */
  measurementCreate({ floatPoint, measurableId, competitorType }) {
    return this.query(this.queries.measurementCreate, {
      input: {
        value: { floatPoint },
        measurableId,
        competitorType,
      },
    });
  }

  /**
   * @public
   * @return {*}
   */
  measurementCreateAggregation({ measurableId, ...rest }) {
    return this.measurementCreate({
      measurableId,
      competitorType: 'AGGREGATION',
      ...rest
    });
  }

  /**
   * @todo: use streams
   * @public
   * @return {*}
   */
  async measurements({ measurableId, competitorType, createdAfter }) {
    const result = await this.query(this.queries.measurements, {
      measurableId,
      competitorType,
      createdAfter,
    });
    return this.getList('measurements')(result);
  }

  /**
   * @public
   * @return {*}
   */
  async measurableAggregate({ measurableId }) {
    const result = await this.query(this.queries.measurableUpdate, {
      measurableId,
      input: { aggregatedAt: new Date() }
    });
    return this.getOne('measurableUpdate')(result);
  }

  /**
   * @public
   * @return {*}
   */
  async measurementsForAggregation({ measurableId }) {
    return this.measurements({
      measurableId,
      competitorType: ['COMPETITIVE'],
      createdAfter: new Date(),
    });
  }

  /**
   * @protected
   * @param {string} alias
   * @return {Function}
   */
  getList(alias) {
    return (result) => result.data[alias].edges.map(edge => edge.node);
  }

  /**
   * @protected
   * @param {string} alias
   * @return {Function}
   */
  getOne(alias) {
    return (result) => result.data[alias];
  }
}

module.exports = {
  API,
};

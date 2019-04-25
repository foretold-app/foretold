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
   * @public
   * @return {*}
   */
  async measurables() {
    const result = await this.query(this.queries.measurables);
    return this.getList('measurables')(result);
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
  async measurements({ measurableId, competitorType }) {
    const result = await this.query(this.queries.measurements, {
      measurableId,
      competitorType
    });
    return this.getList('measurements')(result);
  }

  /**
   * @protected
   * @param {string} alias
   * @return {Function}
   */
  getList(alias) {
    return (result) => result.data[alias].edges.map(edge => edge.node);
  }
}

module.exports = {
  API,
};

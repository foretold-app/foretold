const _ = require('lodash');

/**
 * @param {*[]} result
 * @param {object} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {{total: *, pageInfo: {hasNextPage: boolean, hasPreviousPage: boolean, endCursor: *, startCursor: *}, edges: *}}
 */
function formatResponseIntoConnection(result, root, args, context, info) {
  const total = _.get(result, 'total');

  const edges = result.map(node => ({ node, cursor: node.index }));

  const start = _.head(edges);
  const end = _.last(edges);

  const startCursor = _.get(start, 'cursor');
  const endCursor = _.get(end, 'cursor');

  const hasNextPage = _.toNumber(endCursor) < (total - 1);
  const hasPreviousPage = _.toNumber(startCursor) > 0;

  return {
    total,
    edges,
    pageInfo: {
      hasNextPage,
      hasPreviousPage,
      startCursor,
      endCursor,
    },
  };
}

module.exports = {
  formatResponseIntoConnection,
};

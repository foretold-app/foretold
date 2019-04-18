const _ = require('lodash');

/**
 * @param {*[]} result
 * @param {object} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {{total: *, pageInfo: {hasNextPage: boolean, hasPreviousPage: boolean, endCursor: *, startCursor: *}, edges: *}}
 */
function connection(result, root, args, context, info) {
  const total = context.total;

  const last = _.get(args, 'last', 0);
  const count = result.length;

  const start = _.head(result);
  const end = _.last(result);

  const startCursor = _.get(start, 'createdAt');
  const endCursor = _.get(end, 'createdAt');

  const hasNextPage = (last + count) < total;
  const hasPreviousPage = !!last;

  const edges = result.map(o => ({ node: o, cursor: _.get(o, 'createdAt') }));

  return {
    total: total,
    pageInfo: {
      hasNextPage: hasNextPage,
      hasPreviousPage: hasPreviousPage,
      startCursor: startCursor,
      endCursor: endCursor,
    },
    edges,
  };
}

module.exports = {
  connection,
};

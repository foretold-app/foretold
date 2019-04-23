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

  const edges = result.map(node => ({ node, cursor: node.index }));

  const start = _.head(edges);
  const end = _.last(edges);

  const startCursor = _.get(start, 'cursor');
  const endCursor = _.get(end, 'cursor');

  const hasNextPage = _.toNumber(endCursor) < (total - 1);
  const hasPreviousPage = _.toNumber(startCursor) > 0;

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

/**
 * @param {object} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 */
async function connectionArguments(root, args, context, info) {
  const beforeCursor = _.get(args, 'before', 0);
  const afterCursor = _.get(args, 'after', 0);

  const before = beforeCursor * 1;
  const after = afterCursor * 1;

  args.before = before;
  args.after = after;

  args.last = _.get(args, 'last', 0);
  args.first = _.get(args, 'first', 0);

  return true;
}

module.exports = {
  connection,
  connectionArguments,
};

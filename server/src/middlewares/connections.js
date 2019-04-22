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
  const count = _.get(result, 'length');

  const first = args.first;
  const after = args.after + 1;

  const last = args.last;
  const before = args.before + 1;

  const hasNextPage = (() => {
    if (first) {
      return (args.after + count) < total;
    } else if (last) {
      return (args.before + count - 1) < total;
    }
    return false;
  })();

  const hasPreviousPage = (() => {
    if (first) {
      return !!args.after;
    } else if (last) {
      return !!args.before;
    }
    return false;
  })();

  const getCursor = (i) => {
    if (first) {
      return i + after;
    } else if (last) {
      return i + before;
    }
    return i;
  };

  const edges = result.map((o, i) => ({
    node: o,
    cursor: getCursor(i),
  }));

  const start = _.head(edges);
  const end = _.last(edges);

  const startCursor = _.get(start, 'cursor');
  const endCursor = _.get(end, 'cursor');

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
 * @param root
 * @param args
 * @param context
 * @param info
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

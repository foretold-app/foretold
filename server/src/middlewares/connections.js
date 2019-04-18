const _ = require('lodash');

/**
 *
 * @param {{id: string, total: number}[]} all
 * @param {objects} args
 * @return {{total: *, pageInfo: {hasNextPage: boolean, hasPreviousPage: boolean, endCursor: *, startCursor: *}, edges: *}}
 */
function connection(all, args) {
  const last = _.get(args, 'last', 0);
  const count = all.length;

  const start = _.head(all);
  const end = _.last(all);

  const startCursor = _.get(start, 'id');
  const endCursor = _.get(end, 'id');

  const total = _.get(all, [0, 'total'], 0);
  const hasNextPage = (last + count) < total;
  const hasPreviousPage = !!last;

  const edges = all.map(o => ({ node: o, cursor: _.get(o, 'id') }));

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

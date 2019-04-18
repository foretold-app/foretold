const _ = require('lodash');

/**
 *
 * @param {{data: *, total: number}} result
 * @param {objects} args
 * @return {{total: *, pageInfo: {hasNextPage: boolean, hasPreviousPage: boolean, endCursor: *, startCursor: *}, edges: *}}
 */
function connection(result, args) {
  const { data, total } = result;

  const last = _.get(args, 'last', 0);
  const count = data.length;

  const start = _.head(data);
  const end = _.last(data);

  const startCursor = _.get(start, 'createdAt');
  const endCursor = _.get(end, 'createdAt');

  const hasNextPage = (last + count) < total;
  const hasPreviousPage = !!last;

  const edges = data.map(o => ({ node: o, cursor: _.get(o, 'createdAt') }));

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

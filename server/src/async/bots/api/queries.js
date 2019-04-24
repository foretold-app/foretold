const measurables = `
query measurables {
  measurables(first: 500, states: [OPEN, JUDGEMENT_PENDING], isArchived: [FALSE]) {
      total
      edges {
        node {
          id
          name
          valueType
          state
          Measurements(first:1){
            edges {
               node {
                 id
                 value {
                    floatCdf { xs ys }
                    floatPoint
                 }
               }
            }
          }
      }
    }
  }
}
`;

const queryExample = `
query search ($query: String, $from: Int, $limit: Int) {
  search(query: $query, from: $from, limit: $limit) {
    took,
    totalHits,
    hits {
      name
    }
  }
}
`;

module.exports = {
  measurables,
};

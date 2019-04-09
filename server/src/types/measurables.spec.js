const graphql = require('graphql');
const measurables = require('./measurables');

describe('Measurables Types', () => {
  it('measurables is an object', () => {
    expect(measurables).toBeInstanceOf(Object);
  });
  it('measurableState is an enum', () => {
    expect(measurables.measurableState).toBeInstanceOf(graphql.GraphQLEnumType);
  });
  it('measurable is an object type', () => {
    expect(measurables.measurable).toBeInstanceOf(graphql.GraphQLObjectType);
  });
  it('measurableCreateInput is an input type', () => {
    expect(measurables.measurableCreateInput)
      .toBeInstanceOf(graphql.GraphQLInputObjectType);
  });
  it('measurableUpdateInput is an input type', () => {
    expect(measurables.measurableUpdateInput)
      .toBeInstanceOf(graphql.GraphQLInputObjectType);
  });
});

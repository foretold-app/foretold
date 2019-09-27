const { NewMeasurementComment } = require('./new-measurement-comment');

describe('New Measurement Comment Feed Item Producer', () => {
  it('returns an constructor', () => {
    expect(NewMeasurementComment).toBeInstanceOf(Function);
  });
});

const index = require('./index');

const { MiddlewareGenerator } = require('graphql-middleware/dist/generator');

describe('authorizers index', () => {

  describe('index', () => {
    it('index', () => {
      expect(index).toBeInstanceOf(Object);
      expect(index.permissions).toBeInstanceOf(MiddlewareGenerator);
    });
  });

});

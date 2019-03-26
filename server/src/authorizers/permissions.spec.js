const { shield } = require('graphql-shield');
const { getPermissions } = require('./permissions');

describe('tests permissions', () => {

  it('shield should be called one time', () => {
    getPermissions();
    expect(shield).toHaveBeenCalledTimes(1);
  });

});

const { shield } = require('graphql-shield');
const { getPermissions } = require('./permissions');

describe('permissions', () => {

  it('calls one time', () => {
    getPermissions();
    expect(shield).toHaveBeenCalledTimes(1);
  });

});

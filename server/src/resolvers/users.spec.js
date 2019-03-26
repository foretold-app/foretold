const users = require('./users');
const data = require('../data');

describe('users', () => {
  afterEach(() => {
    jest.clearAllMocks();
    jest.restoreAllMocks();
  });

  describe('users', () => {
    const root = {};
    const args = {};
    const context = {};
    beforeAll(() => {
      jest.spyOn(data.usersData, 'editUser').mockReturnValue(Promise.resolve(true));
    });
    it('all', () => {
      return users.edit(root, args, context).then((result) => {
        expect(data.usersData.editUser).toHaveBeenCalledWith(
          root, args, context,
        );
        expect(result).toBe(true);
      });
    });
  });


});

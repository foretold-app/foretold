const users = require('./users');
const data = require('../data');

describe('users', () => {

  describe('users', () => {
    const root = {};
    const args = {};
    const context = {};
    beforeEach(() => {
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

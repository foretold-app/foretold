const models = require('../models');
const { UsersData } = require('./users-data');

describe('Users Data Layer', () => {

  it('class should be constructor', () => {
    expect(UsersData).toBeInstanceOf(Function);
  });

  const instance = new UsersData();
  const auth0Id = 'auth0Id';

  describe('createOne()', () => {
    beforeEach(() => {
      jest.spyOn(models.User, 'findOne').mockReturnValue(Promise.resolve(true));
      jest.spyOn(models.User, 'create').mockReturnValue(Promise.resolve(true));
    });
    it('finds user', () => {
      return instance.getUserByAuth0Id(auth0Id).then((result) => {
        expect(models.User.findOne).toHaveBeenCalledWith({
          where: { auth0Id },
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('createOne()', () => {
    beforeEach(() => {
      jest.spyOn(models.User, 'findOne').mockReturnValue(Promise.resolve(false));
      jest.spyOn(models.User, 'create').mockReturnValue(Promise.resolve(true));
    });
    it('creates user when he is found', () => {
      return instance.getUserByAuth0Id(auth0Id).then((result) => {
        expect(models.User.create).toHaveBeenCalledWith({
          auth0Id: auth0Id, name: ''
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('editUser()', () => {
    const root = {};
    const values = { id: 'id1', name: 'name1' };
    const options = { user: { auth0Id: 'auh0Id2' } };
    const update = jest.fn(() => Promise.resolve(true));
    beforeEach(() => {
      jest.spyOn(models.User, 'findById').mockReturnValue(Promise.resolve({
        auth0Id: 'auh0Id2',
        update
      }));
    });
    it('finds user and update him', () => {
      return instance.editUser(root, values, options).then((result) => {
        expect(models.User.findById).toHaveBeenCalledWith(values.id);
        expect(update).toHaveBeenCalledWith({ name: values.name });
      });
    });
  });

  describe('getUser()', () => {
    const root = {};
    const values = { id: 'id1', auth0Id: 'auth0Id1' };
    const options = { user: { auth0Id: 'auh0Id2' } };
    it('returns user when options are set', () => {
      return instance.getUser(root, values, options).then((result) => {
        expect(result).toBe(options.user);
      });
    });
  });

  describe('getUser()', () => {
    const root = {};
    const values = { id: 'id1', auth0Id: 'auth0Id1' };
    const options = {};
    beforeEach(() => {
      jest.spyOn(models.User, 'findById').mockReturnValue(Promise.resolve(true));
    });
    it('finds user', () => {
      return instance.getUser(root, values, options).then((result) => {
        expect(result).toBe(true);
      });
    });
  });

  describe('getUser()', () => {
    const root = {};
    const values = { auth0Id: 'auth0Id1' };
    const options = {};
    beforeEach(() => {
      jest.spyOn(instance, 'getUserByAuth0Id').mockReturnValue(Promise.resolve(true));
    });
    it('finds user by auth0Id', () => {
      return instance.getUser(root, values, options).then((result) => {
        expect(result).toBe(true);
      });
    });
  });

});

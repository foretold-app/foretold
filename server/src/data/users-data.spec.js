jest.mock('../models');

const models = require('../models');
const { UsersData } = require('./users-data');

describe('UsersData', () => {
  afterEach(() => {
    jest.clearAllMocks();
    jest.restoreAllMocks();
  });

  it('class should be constructor', () => {
    expect(UsersData).toBeInstanceOf(Function);
  });

  const instance = new UsersData();
  const auth0Id = 'auth0Id';

  describe('createOne - branch A', () => {
    beforeAll(() => {
      jest.spyOn(models.User, 'findOrCreate').mockReturnValue(Promise.resolve(true));
    });
    it('createOne', () => {
      return instance.getUserByAuth0Id(auth0Id).then((result) => {
        expect(models.User.findOrCreate).toHaveBeenCalledWith({
          where: { auth0Id },
          defaults: { auth0Id: auth0Id, name: '' },
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('editUser - branch A', () => {
    const root = {};
    const values = { id: 'id1', name: 'name1' };
    const options = { user: { auth0Id: 'auh0Id2' } };
    const update = jest.fn(() => Promise.resolve(true));
    beforeAll(() => {
      jest.spyOn(models.User, 'findById').mockReturnValue(Promise.resolve({
        auth0Id: 'auh0Id2',
        update
      }));
    });
    it('createOne', () => {
      return instance.editUser(root, values, options).then((result) => {
        expect(models.User.findById).toHaveBeenCalledWith(values.id);
        expect(update).toHaveBeenCalledWith({ name: values.name });
      });
    });
  });

  describe('getUser - branch A', () => {
    const root = {};
    const values = { id: 'id1', auth0Id: 'auth0Id1' };
    const options = { user: { auth0Id: 'auh0Id2' } };
    it('createOne', () => {
      return instance.getUser(root, values, options).then((result) => {
        expect(result).toBe(options.user);
      });
    });
  });

  describe('getUser - branch B', () => {
    const root = {};
    const values = { id: 'id1', auth0Id: 'auth0Id1' };
    const options = {};
    beforeAll(() => {
      jest.spyOn(models.User, 'findById').mockReturnValue(Promise.resolve(true));
    });
    it('createOne', () => {
      return instance.getUser(root, values, options).then((result) => {
        expect(result).toBe(true);
      });
    });
  });

  describe('getUser - branch C', () => {
    const root = {};
    const values = { auth0Id: 'auth0Id1' };
    const options = {};
    beforeAll(() => {
      jest.spyOn(instance, 'getUserByAuth0Id').mockReturnValue(Promise.resolve(true));
    });
    it('createOne', () => {
      return instance.getUser(root, values, options).then((result) => {
        expect(result).toBe(true);
      });
    });
  });

});

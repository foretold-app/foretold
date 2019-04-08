jest.mock('./models/index');
jest.mock('./lib/notifications');
jest.mock('./models-abstract/index', () => require('./models-abstract/__mocks__/models-abstract'));

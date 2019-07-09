/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class Model {

  getAll() {
    return Promise.reject(new Error('getAll is not implemented.'));
  }

  getOne() {
    return Promise.reject(new Error('getOne is not implemented.'));
  }

  createOne() {
    return Promise.reject(new Error('createOne is not implemented.'));
  }

  updateOne() {
    return Promise.reject(new Error('updateOne is not implemented.'));
  }

  updateAll() {
    return Promise.reject(new Error('updateAll is not implemented.'));
  }

  upsertOne() {
    return Promise.reject(new Error('upsertOne is not implemented.'));
  }
}

module.exports = {
  Model,
};

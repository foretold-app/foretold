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

  deleteOne() {
    return Promise.reject(new Error('deleteOne is not implemented.'));
  }
}

module.exports = {
  Model,
};

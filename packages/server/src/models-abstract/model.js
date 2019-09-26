/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class Model {
  getTransaction() {
    return Promise.reject(new Error('getTransaction is not implemented.'));
  }

  commit(transaction) {
    return Promise.reject(new Error('getTransaction is not implemented.'));
  }

  rollback(transaction) {
    return Promise.reject(new Error('rollback is not implemented.'));
  }

  getAll(filter, pagination, restrictions, options) {
    return Promise.reject(new Error('getAll is not implemented.'));
  }

  getAllWithConnections(filter, pagination, restrictions, options) {
    return Promise.reject(new Error('getAllWithConnections is not implemented.'));
  }

  getOne(params, query, restrictions, options) {
    return Promise.reject(new Error('getOne is not implemented.'));
  }

  createOne(data, restrictions, options) {
    return Promise.reject(new Error('createOne is not implemented.'));
  }

  updateOne(params, data, restrictions, options) {
    return Promise.reject(new Error('updateOne is not implemented.'));
  }

  deleteOne(params, restrictions, options) {
    return Promise.reject(new Error('deleteOne is not implemented.'));
  }

  updateAll(params, data, restrictions, options) {
    return Promise.reject(new Error('updateAll is not implemented.'));
  }

  upsertOne(params, query, data, restrictions, options) {
    return Promise.reject(new Error('upsertOne is not implemented.'));
  }
}

module.exports = {
  Model,
};

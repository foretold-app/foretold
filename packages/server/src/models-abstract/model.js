/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class Model {
  getTransaction() {
    return Promise.reject(new Error('getTransaction is not implemented.'));
  }

  commit(_transaction) {
    return Promise.reject(new Error('getTransaction is not implemented.'));
  }

  rollback(_transaction) {
    return Promise.reject(new Error('rollback is not implemented.'));
  }

  getAll(_filter, _pagination, _restrictions, _options) {
    return Promise.reject(new Error('getAll is not implemented.'));
  }

  getAllWithConnections(_filter, _pagination, _restrictions, _options) {
    return Promise.reject(
      new Error('getAllWithConnections is not implemented.'),
    );
  }

  getOne(_params, _query, _restrictions, _options) {
    return Promise.reject(new Error('getOne is not implemented.'));
  }

  createOne(_data, _restrictions, _options) {
    return Promise.reject(new Error('createOne is not implemented.'));
  }

  updateOne(_params, _data, _restrictions, _options) {
    return Promise.reject(new Error('updateOne is not implemented.'));
  }

  deleteOne(_params, _restrictions, _options) {
    return Promise.reject(new Error('deleteOne is not implemented.'));
  }

  updateAll(_params, _data, _restrictions, _options) {
    return Promise.reject(new Error('updateAll is not implemented.'));
  }

  upsertOne(_params, _query, _data, _restrictions, _options) {
    return Promise.reject(new Error('upsertOne is not implemented.'));
  }
}

module.exports = {
  Model,
};

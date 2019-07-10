class Filter {
  /**
   * @param {Layers.DataSourceLayer.filter} [filter]
   */
  constructor(filter = {}) {
    this.type = filter.type;
  }
}

module.exports = {
  Filter,
};

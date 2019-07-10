class Options {
  /**
   * @param {Layers.DataSourceLayer.options} [options]
   */
  constructor(options = {}) {
    this.type = options.transaction;
  }
}

module.exports = {
  Options,
};

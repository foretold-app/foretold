export var Sampler = {
  /**
   * @param value
   * @returns {{values: [*]}}
   */
  sample({ params: [value] }) {
    return ({ values: [value] });
  }
};

const { Cdf } = require('./');

class Pdf {
  /**
   * @param {number[]} xs
   * @param {number[]} ys
   */
  constructor(xs, ys) {
    if (!this.validateSize(xs, ys)) {
      throw new Error('Arrays of "xs" and "ys" have different sizes.');
    }

    if (!this.validateHasLength(xs)) {
      throw new Error('You need at least one element.');
    }

    const sorted = this.order(xs, ys);
    this.xs = sorted.xs;
    this.ys = sorted.ys;
  }

  /**
   * @param xs
   * @param ys
   * @return {boolean}
   */
  validateSize(xs, ys) {
    return xs.length === ys.length;
  }

  /**
   * @param xs
   * @return {boolean}
   */
  validateHasLength(xs) {
    return xs.length > 0;
  }

  /**
   * @param xs
   * @param ys
   * @return {{ys: *, xs: *}}
   */
  order(xs, ys) {
    const xsYs = xs.map((v, i) => ({ ys: ys[i], xs: v }));
    const sorted = xsYs.sort((a, b) => {
      if (a.xs > b.xs) return 1;
      if (a.xs < b.xs) return -1;
      return 0;
    });

    const XS = sorted.map(v => v.xs);
    const YS = sorted.map(v => v.ys);

    return { xs: XS, ys: YS };
  }

  /**
   * @return {Cdf}
   */
  toCdf() {
    let newYs = [this.ys[0]];
    for (let i = 1; i < this.ys.length; i++) {
      newYs.push(newYs[i - 1] + this.ys[i])
    }
    return new (require('./cdf').Cdf)(this.xs, newYs);
  }
}

module.exports = {
  Pdf,
};

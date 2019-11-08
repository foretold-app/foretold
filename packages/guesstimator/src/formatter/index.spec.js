import { _matchingFormatter, formatters } from './index';

describe('formatter', () => {
  describe('#formatters', () => {
    it('holds all formatters', () => {
      expect(formatters.length)
        .toBeGreaterThan(3);
    });
  });

  describe('#_matchingFormatter', () => {
    const examples = [
      [{ text: '=34' }, 'FUNCTION'],
      [{ text: '34' }, 'DISTRIBUTION_POINT_TEXT'],
      [{ text: '34 -> 88' }, 'DISTRIBUTION_NORMAL_TEXT_UPTO']
    ];

    examples.map(e => () => {
      it(`finds correct formatter for input ${JSON.stringify(e[0])}`, () => {
        expect(_matchingFormatter(e[0]).formatterName)
          .toBe(e[1]);
      });
    })
      .map(e => e());
  });
});

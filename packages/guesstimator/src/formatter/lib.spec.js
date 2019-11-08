import { regexBasedFormatter, shorthandIntoLognormalFormattingStep } from './lib.js'

describe('Lib', () => {
  it('returns as an object', () => {
    expect(regexBasedFormatter).toBeInstanceOf(Function);
  });
});

describe('shorthandIntoLognormalFormattingStep', () => {
  const examples = [
    ['1 to 10', 'lognormal(1.151,0.700)'],
    ['1K to 10K', 'lognormal(8.059,0.700)' ],
    ['-1 to 10', "normal(4.500,3.343)"],
    ['1->10', 'lognormal(1.151,0.700)'],
    ['1:10', 'lognormal(1.151,0.700)'],
    ['=mm(1 to 10, normal(1,1),[.1,.3])', '=mm(lognormal(1.151,0.700), normal(1,1),[.1,.3])'],
    ['=mm(1K to 10K, normal(1,1),[.1,.3])', '=mm(lognormal(8.059,0.700), normal(1,1),[.1,.3])' ],
    ['=mm(-1 to 10, normal(1,1),[.1,.3])', "=mm(normal(4.500,3.343), normal(1,1),[.1,.3])"],
    ['=mm(1->10, normal(1,1),[.1,.3])', '=mm(lognormal(1.151,0.700), normal(1,1),[.1,.3])'],
    ['=mm(1:10, normal(1,1),[.1,.3])', '=mm(lognormal(1.151,0.700), normal(1,1),[.1,.3])'],
  ]

  examples.map(e => () => {
    it(`it converts ${e[0]} to ${e[1]}`, () => {
      expect(shorthandIntoLognormalFormattingStep(e[0])).toEqual(e[1])
    })
  }).map(e => e())
})
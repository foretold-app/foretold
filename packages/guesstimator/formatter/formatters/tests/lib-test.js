import {shorthandIntoLognormalFormattingStep} from '../lib.js'

describe('shorthandIntoLognormalFormattingStep', () => {
  const examples = [
    ['1 to 10', 'lognormal(1.151, 0.700)'],
    ['1K to 10K', 'lognormal(9.210, 0.700)' ],
    ['-1 to 10', "[unknown]"],
    ['1->10', 'lognormal(1.151, 0.700)'],
    ['1:10', 'lognormal(1.151, 0.700)'],
    ['=mm(1 to 10, normal(1,1),[.1,.3])', '=mm(lognormal(1.151, 0.700), normal(1,1),[.1,.3])'],
    ['=mm(1K to 10K, normal(1,1),[.1,.3])', '=mm(lognormal9.210.000, 0.700), normal(1,1),[.1,.3])' ],
    ['=mm(-1 to 10, normal(1,1),[.1,.3])', "[unknown]"],
    ['=mm(1->10, normal(1,1),[.1,.3])', '=mm(lognormal(1.151, 0.700), normal(1,1),[.1,.3])'],
    ['=mm(1:10, normal(1,1),[.1,.3])', '=mm(lognormal(1.151, 0.700), normal(1,1),[.1,.3])'],
  ]

  examples.map(e => () => {
    it(`it converts ${e[0]} to ${e[1]}`, () => {
      expect(shorthandIntoLognormalFormattingStep(e[0])).to.equal(e[1])
    })
  }).map(e => e())
})
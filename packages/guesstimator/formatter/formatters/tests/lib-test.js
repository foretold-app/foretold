import {shorthandIntoLognormalFormattingStep} from '../lib.js'

describe('shorthandIntoLognormalFormattingStep', () => {

  describe('Behavior:', () => {
    const examples = [
      ['1 to 10', 'lognormal(5.500, 0.700)'],
      ['1K to 10K', 'lognormal(5500.000, 0.700)' ],
      ['-1 to 10', "[unknown]"],
      ['1-10', 'lognormal(5.500, 0.700)'],
      ['1:10', 'lognormal(5.500, 0.700)'],
    ]

    examples.map(e => () => {
      it(`shorthandIntoLognormalFormattingStep takes ${e[0]} and converts it to ${shorthandIntoLognormalFormattingStep(e[0])}. It should convert it to ${e[1]}`, () => {
        expect(shorthandIntoLognormalFormattingStep(e[0])).to.equal(e[1])
      })
    }).map(e => e())
  });
})

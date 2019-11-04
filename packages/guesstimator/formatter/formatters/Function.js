import _ from "lodash";
import {shorthandIntoLognormalFormattingStep} from './lib'

//FIXME

// The function: shorthandIntoLognormalFormattingStep
// transforms strings like "=mm(normal(10,5), 1 to 100))
// into strings like "=mm(normal(10,5), lognormal(50.1, 1.4))

export const item = {
  formatterName: 'FUNCTION',
  matches({text}) { return !!text && text.startsWith('=') },
  error({text}) { return !_.isEmpty(text.slice(1)) ? {} : {type: 1, subType: 2}},
  format({text}) { return {guesstimateType: 'FUNCTION', text: (shorthandIntoLognormalFormattingStep(text)).slice(1)} },
}

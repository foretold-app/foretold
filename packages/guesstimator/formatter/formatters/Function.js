import _ from "lodash";
import {regexNumberToNumberIntoLognormal} from './lib'

//FIXME
export const item = {
  formatterName: 'FUNCTION',
  matches({text}) { return !!text && text.startsWith('=') },
  error({text}) { return !_.isEmpty(text.slice(1)) ? {} : {type: 1, subType: 2}},
  format({text}) { return {guesstimateType: 'FUNCTION', text: (NumberToNumberIntoLognormal(text)).slice(1)} },
}

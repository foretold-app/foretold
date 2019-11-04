import _ from "lodash";
import {NumberToNumberIntoLognormal} from './lib'

//FIXME
export const item = {
  formatterName: 'FUNCTION',
  matches({text}) { return !!text && text.startsWith('=') },
  error({text}) { return !_.isEmpty(text.slice(1)) ? {} : {type: 1, subType: 2}},
  format({text}) { return {guesstimateType: 'FUNCTION', text: (NumberToNumberIntoLognormal(text)).slice(1)} },
      // text: (NumberToNumberIntoLognormal(text)).slice(1)
      //        Allows the program to parse an input of the form "=mm(normal(1,10), 1 to 100)"
      // text: text.slice(1)
      //        Doesn't allow the program to parse an input of the form "=mm(normal(1,10), 1 to 100)"
         
}

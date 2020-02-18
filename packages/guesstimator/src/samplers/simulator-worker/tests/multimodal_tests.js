import { Guesstimator } from '../../../index.js';

// These tests are very manual atm and could definitely be improved
const bimodal_tests = [
  {
    test: '=bi(9,2)',
    output: 'See if the output values look fine',
    id: 0
  },

  {
    test: '=bi(9,2,20)',
    output: 'RangeError',
    id: 1
  },

  {
    test: '=bi(9,2,20,5)',
    output: 'See if the output values look fine',
    id: 2
  },

  {
    test: '=bi(9,normal(0,2),0.7)',
    output: 'See if the output values look fine',
    id: 3
  },

  {
    test: '=bi(9,lognormal(1,2),[0.4])',
    output: 'See if the output values look fine',
    id: 4
  },

  {
    test: '=bi(9,normal(98,1),[0.2, 0.8])',
    output: 'See if the output values look fine',
    id: 5
  },

  {
    test: '=bi(9,normal(-10,2),-0.4)',
    output: 'RangeError',
    id: 6
  },

  {
    test: '=bi(9,AK,0.3)',
    output: 'See if the output values look fine',
    id: 7
  },

  {
    test: '=bi(BK,AK,[0.7])',
    output: 'See if the output values look fine',
    id: 8
  }
];

const multimodal_tests = [
  {
    test: '=multimodal(1,2,3)',
    output: 'See if the output values look fine',
    id: 0
  },

  {
    test: '=mm(normal(1,2),normal(233,1),normal(190,4), normal(1000,1))',
    output: 'See if the output values look fine',
    id: 1
  },

  {
    test: '=mm(normal(1,2),normal(233,1),normal(190,4), normal(1000,1), [1,100,20,1])',
    output: 'See if the output values look fine',
    id: 2
  },

  {
    test: '=mm(normal(1,2),normal(233,1),normal(190,4), normal(1000,1), [1,100,20])',
    output: 'RangeError',
    id: 3
  },

  {
    test: '=mm(normal(1,2), AK, BK, [0.3,0.2])',
    output: 'See if the output values look fine',
    id: 4
  },

  {
    test: '=mm("a", AK, BK, [0.3,0.2])',
    output: 'TypeError',
    id: 5
  },

  {
    test: '=mm(3*AK,normal(10*BK,1),100,[randomInt(0,100)/100])',
    output: 'See if the output values look fine',
    id: 6
  }
];

// Input cells
let input2 = { text: '=bimodal(9,2)' }; // Ideally this should be more flexible
let [error2, item2] = Guesstimator.parse(input2);
let parsedInput2 = item2.parsedInput;
let f = new Guesstimator({ parsedInput: parsedInput2 });
let bk = f.sample(1000);
const externalInputs = { AK: [55, 56, 57], BK: [1, 2, 3] }; // BK: bk.values};

console.log(
  '___\n\n ***Running bi and multimodal functions against some test cases**\n',
);
for (var suite of [bimodal_tests, multimodal_tests]) {// .forEach( suite => {
  var t = 0;
  suite.forEach(Test => {// Main cell

      // let string = bimodal_tests[t].test
      let input = { text: Test.test };
      let [error, item] = Guesstimator.parse(input);
      let parsedInput = item.parsedInput;
      let g = new Guesstimator({ parsedInput: parsedInput });

      console.log('Test number ' + t + ':\n', input, ':\n' +
        'Intended output: ', Test.output, '\n' +
        'Actual output: ', g.sample(20, externalInputs), '\n\n');

      t++;
    }
  );
}

// Note 1: Foretold is not set up to test with cells initially, so one has to
// turn some placeholders arguments into variables
// in other files to do that.
// Note 2: Only tested with Foretold Guesstimator codebase, not Guesstimate yet.
// Note 3: to run this file in VS code with node and ES6, I created a separate
// js file and ran that one, with contents:
// require = require("esm")(module /*, options*/);
// module.exports = require("./multimodal_tests.js");

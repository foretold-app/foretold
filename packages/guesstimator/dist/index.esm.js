import _ from 'lodash';
import math from 'mathjs';
import { jStat } from 'jstat';
import Sampling, { Discrete } from 'discrete-sampling';

//FIXME
const item = {
  formatterName: 'FUNCTION',
  matches({text}) { return !!text && text.startsWith('=') },
  error({text}) { return !_.isEmpty(text.slice(1)) ? {} : {type: 1, subType: 2}},
  format({text}) { return {guesstimateType: 'FUNCTION', text: text.slice(1)} },
};

const SUFFIXES = {
  '%': -2,
  'K': 3,
  'M': 6,
  'B': 9,
  'T': 12,
};

const parenthesize = str => `(?:${str})`;
//TODO
const escSpecialChars = str => str.replace(/\$|\{|\}|\_/g, e => `\\${e}`);
const isPresent = e => (!!e && !_.isEmpty(e)) || (typeof e === 'number') || (e === true);
const toSource = re => re instanceof RegExp ? re.source : escSpecialChars(re);
function or(res) {
  const strParts = res.map(toSource).filter(isPresent).map(parenthesize);
  const lengthSorted = _.sortBy(strParts, prt => -prt.length); // To avoid partial replacements.
  return new RegExp(parenthesize(lengthSorted.join('|')), 'g')
}

const spaceSep = res => new RegExp(res.filter(re => !!re).map(re => `(?:${re.source})`).join('\\s*'));
const padded = res => spaceSep([/^/, ...res, /$/]);

const SUFFIX_REGEX = new RegExp(Object.keys(SUFFIXES).join('|'));
const INTEGER_REGEX = /(?:(?:\d+)|(?:\d{1,3}(?:,\d{3})*))(?!\.[^\.])/;
const DECIMAL_REGEX = /\d*\.\d+/;
const NUMBER_REGEX = new RegExp(`(-?${or([INTEGER_REGEX, DECIMAL_REGEX]).source})\\s?(${SUFFIX_REGEX.source})?`);

const POINT_REGEX = padded([NUMBER_REGEX]);
const rangeRegex = (sep, left, right) => padded([left, NUMBER_REGEX, sep, NUMBER_REGEX, right]);

const getMult = suffix => Math.pow(10,SUFFIXES[suffix]);
const parseNumber = (num, suffix) => parseFloat(num.replace(',', '')) * (!!suffix ? getMult(suffix) : 1);

const rangeErrorFn = ([low, high]) => low > high ? {type: 1, subType: 2} : {};

// We assume that if the user started at 0 or tried a negative number,
// they intended for this to be normal.
function getGuesstimateType(guesstimateType, [low]) {
  switch (guesstimateType) {
    case 'UNIFORM': return guesstimateType
    case 'NORMAL': return guesstimateType
    default: return low <= 0 ? 'NORMAL' : 'LOGNORMAL'
  }
}

function regexBasedFormatter(re, guesstimateTypeFn = getGuesstimateType, errorFn = rangeErrorFn) {
  return {
    matches({text}) { return re.test(text) },
    error({text}) { return errorFn(this._numbers(text)) },

    format({guesstimateType, text}) {
      const params = this._numbers(text);
      return {guesstimateType: guesstimateTypeFn(guesstimateType, params), params}
    },

    _numbers(text) { return _.chunk(text.match(re).slice(1), 2).map(([num, suffix]) => parseNumber(num, suffix)) },
  }
}

const item$1 = {
  formatterName: 'DISTRIBUTION_NORMAL_TEXT_UPTO',
  ...regexBasedFormatter(rangeRegex(/to|\.\.|->|:/)),
};

const item$2 = {
  formatterName: 'DISTRIBUTION_NORMAL_TEXT_UPTO',
  ...regexBasedFormatter(rangeRegex(/,\s?/, /\[/, /\]/)),
};

const item$3 = {
  formatterName: 'DISTRIBUTION_PROPORTIONALITY',
  ...regexBasedFormatter(rangeRegex(/of|in/), () => 'BETA'),
};

const item$4 = {
  formatterName: 'DISTRIBUTION_POINT_TEXT',
  ...regexBasedFormatter(POINT_REGEX, () => 'POINT', () => {}),
};

const item$5 = {
  formatterName: 'DATA',
  error(g) { return {} },
  matches(g) { return !_.isEmpty(g.data) },
  format(g) { return { guesstimateType: 'DATA', data: g.data } },
};

//TODOFIX ERRORS
const item$6 = {
  guesstimateType: 'NONE',
  inputType: 'NONE',
  formatterName: 'NULL',
  matches(g) { return true },
  format(g) { return {guesstimateType: 'NONE'} },
  error({text}) { return _.isEmpty(text) ? {} : {type: 1, subType: 2} },
};

const formatters = [
  item,
  item$1,
  item$3,
  item$2,
  item$4,
  item$5
];

function _matchingFormatter(g) {
  for (let formatter of formatters) {
    if (formatter.matches(g)) {
      return formatter
    }
  }
  return item$6
}

// General formatting that applies to everything.  After it goes through
// this stage, a specific formatter gets applied.
function prepare(guesstimate) {
  return {
    text: (guesstimate.input || guesstimate.text),
    guesstimateType: guesstimate.guesstimateType,
    data: (guesstimate.data || guesstimate.value)
  }
}

function parse(g) {
  const i = prepare(g);
  const formatter = _matchingFormatter(i);
  return [formatter.error(i), formatter.format(i)]
}

// We use an empty object here instead of a more meaningful object to play well with mathjs under the hood.
const SAMPLE_FILTERED = {filtered: true};

const filterLessThan = (val, min) => val < min ? SAMPLE_FILTERED : val;
const filterGreaterThan = (val, max) => val > max ? SAMPLE_FILTERED : val;
const filterBetween = (val, min, max) => val < min || val > max ? SAMPLE_FILTERED : val;

const Filters = {
  filterLessThan,
  filterGreaterThan,
  filterBetween,
};

const scenarios = (values, inputProbabilities) => {
  const total = _.sum(inputProbabilities);
  const probabilities = inputProbabilities.map(e => e/total);

  const bound = Math.min(values.length, probabilities.length);
  const testStat = Math.random();

  let running = 0;
  for (var i = 0; i < bound; i++) {
    running += probabilities[i];
    if (testStat < running) {
      return values[i]
    }
  }

  return Math.Nan
};

const ImpureConstructs = {
  scenarios: scenarios,
};

const bernoulli = p => Sampling.Bernoulli(p).draw();
const binomial = (n, p) => Sampling.Binomial(n,p).draw();
const poisson = (lambda) => Sampling.Poisson(lambda).draw();
const negBinomial = (r, p) => Sampling.NegBinomial(r, p).draw();

function triangular(min, max, mode = (min + max)/2) {
  const u = Math.random();
  if (u < (mode-min)/(max-min)) {
    return min + Math.sqrt(u*(max-min)*(mode-min))
  } else {
    return max - Math.sqrt((1-u)*(max-min)*(max-mode))
  }
}

// Source:
// http://www.mhnederlof.nl/doubletriangular.html
function doubleTriangular(min, max, mode = (min + max)/2) {
  const u = Math.random();
  if (u <= 0.5) {
    return min + (mode - min)*Math.sqrt(2*u)
  } else {
    return max - (max - mode)*Math.sqrt(2*(1-u))
  }
}

// Source:
// https://en.wikipedia.org/wiki/Beta_distribution#Transformations
function PERT(min, max, mode = (min + max)/2, lambda = 4) {
  const width = max - min;
  const a = 1 + lambda * ((mode - min)/width);
  const b = 1 + lambda * ((max - mode)/width);
  const p = jStat.beta.sample(a, b);
  return min + p*width
}

const Distributions = {
  beta: jStat.beta.sample,
  centralF: jStat.centralF.sample,
  cauchy: jStat.cauchy.sample,
  chisquare: jStat.chisquare.sample,
  exponential: jStat.exponential.sample,
  invgamma: jStat.invgamma.sample,
  lognormal: jStat.lognormal.sample,
  logn: jStat.lognormal.sample,
  normal: jStat.normal.sample,
  n: jStat.normal.sample,
  studentt: jStat.studentt.sample,
  weibull: jStat.weibull.sample,
  uniform: jStat.uniform.sample,
  gamma: jStat.gamma.sample,
  triangular,
  doubleTriangular,
  PERT,
  bernoulli: bernoulli,
  if: bernoulli,
  test: bernoulli,
  binomial: binomial,
  poisson: poisson,
  negBinomial: negBinomial
};

function bimodal(d1, d2, w=math.matrix([0.5])) {
    // A handy shorthand without the need to add square brackets
    if (!isNaN(w)) { //Check if weights specified as numbers, and if so, turn them into a matrix
        var array_ws = Array.prototype.slice.call(arguments,2);
        w = math.matrix(array_ws);
    }
    return multimodal(d1, d2, w)
}

function multimodal() {
    // Input should take the form: ( d1, d2, d3, ... math.matrix([w1, w2, w3, ...])  where
    // dn are numbers/functions which evaluate to numbers when called, and wn are weights.
    // Weights are optional, and if none are specified, the distribution will be sampled from uniformly.
    // Weights can be specified using:
    //  1) probability of being sampled (e.g. [0.2, 0.4, 0.4]), or
    //  2) any other positive number (e.g. [2, 4, 4] or [1, 2, 2]).
    // However, if only some weights are specified, it will be assumed notation of kind 1) is used, and otherwise
    // an error is thrown.
    // (The math.matrix is there instead of an array as mathjs compiles the expression "[]" that way. Might fix later.)
    var args = Array.from(arguments);
    let l = args.length;

    // check whether user submited an array of weights
    if (math.typeof(args[l-1]) == 'Matrix') {
        // separate distributions and weights into separate arrays (and extract array from mathjs matrix)
        var weights = args.pop()._data;

        // Check that all arguments are numbers
        if (args.some(isNaN)) throw new TypeError("Input must be coercible to numbers (except weight array)")

        // add implicit weights, uniform over the remaining probability mass
        let diff = args.length - weights.length;
        let sum_w = weights.reduce((a, b) => a + b, 0);
        let uniform_weights = Array(diff).fill( (1-sum_w)/diff );

        weights = weights.concat(uniform_weights);

        // check for error
        if (weights.some(v => v < 0)) throw new RangeError(`Some negative weights after normalisation.
        Did you maybe use custom weights > 1 and forget to specify all of them?`)
    }
    else {
      //FIX there should be some kind of TypeError check her as well, but want it DRY
        var weights = Array(l).fill(1/l);
    }

    // Sample index and get corresponding distribution
    let probabilities = Discrete(weights);
    return args[probabilities.draw()]
}

const Multimodals = {
    bimodal,
    bi: bimodal,
    multimodal,
    mm: multimodal,
};

var Finance = require('financejs');

const finance = new Finance();

const SAMPLING_ERROR = 1;
const PARSER_ERROR = 1;
const FUNCTIONS_CONTAIN_UNITS_ERROR = 1;
const UNEXPECTED_END_OF_EXPRESSION_ERROR = 1;
const DIVIDE_BY_ZERO_ERROR = 1;
const ALL_SAMPLES_FILTERED_ERROR = 1;

const financeFunctions = {
  PV: finance.PV,
  FV: finance.FV,
  NPV: finance.NPV,
  //IRR: finance.IRR, Too slow.
  PP: finance.PP,
  ROI: finance.ROI,
  AM: finance.AM,
  PI: finance.PI,
  DF: finance.DF,
  CI: finance.CI,
  CAGR: finance.CAGR,
  LR: finance.LR,
  R72: finance.R72,
  WACC: finance.WACC
};

// Distributions:
math.import(Distributions, {override: true});
// Financial functions:
math.import(financeFunctions, {override: true});
// Guesstimate constructs:
math.import(ImpureConstructs, {override: true, wrap: true});
// Filters
math.import(Filters, {override: true});
// Multimodals
math.import(Multimodals, {override: true});

function Evaluate(text, sampleCount, inputs) {
  try {
    const compiled = math.compile(text);

    return evaluate(compiled, inputs, sampleCount, text)
  } catch ({message}) {
    if (message.startsWith('Unexpected end of expression')) {
      return {errors: [{type: SAMPLING_ERROR, subType: UNEXPECTED_END_OF_EXPRESSION_ERROR, rawMessage: message}]}
    } else {
      return {errors: [{type: SAMPLING_ERROR, rawMessage: message}]}
    }
  }
}

function sampleInputs(inputs, i) {
  const sample = {};
  for (let key of Object.keys(inputs)){
    sample[key] = inputs[key][i % inputs[key].length];
    util.inspect(inputs);
  }
  return sample
}

function evaluate(compiled, inputs, n, text){
  let values = [];
  let errors = [];
  let anyNotFiltered = false;

  for (var i = 0; i < n; i++) {
    const sampledInputs = sampleInputs(inputs, i);
    const someInputFiltered = _.some(sampledInputs, val => _.isEqual(val, SAMPLE_FILTERED));
    let newSample = NaN;
    try {
      newSample = someInputFiltered ? SAMPLE_FILTERED : compiled.eval(sampledInputs);

    } catch (rawError) {
      const isUnexpectedTypeError = rawError.message.includes('Unexpected type of argument in function');
      const containsFilterFn = _.some(Object.keys(Filters), f => text.includes(f));
      if (isUnexpectedTypeError && containsFilterFn) {
        newSample = SAMPLE_FILTERED;
      } else {
        return {values: [], errors: [{type: SAMPLING_ERROR, rawError}]}
      }
    }

    if (_.isFinite(newSample)) {
      anyNotFiltered = true;
      values.push(newSample);
    } else if (newSample === SAMPLE_FILTERED) {
      values.push(newSample);
    } else if ([Infinity, -Infinity].includes(newSample)) {
      errors.push({type: SAMPLING_ERROR, subType: DIVIDE_BY_ZERO_ERROR});
      values.push(newSample);
    } else if (newSample.constructor.name === 'Unit') {
      return {values: [], errors: [{type: PARSER_ERROR, subType: FUNCTIONS_CONTAIN_UNITS_ERROR}]}
    } else if (typeof newSample === 'function') {
      return {values: [], errors: [{type: PARSER_ERROR, subType: INCOMPLETE_FUNCTION_ERROR}]}
    } else {
      if (__DEV__) { console.warn('Unidentified sample detected: ', newSample); }
      return {values: [], errors: [{type: SAMPLING_ERROR}]}
    }
  }

  errors = _.uniq(errors);

  return anyNotFiltered ? {values, errors} : {values: [], errors: [...errors, {type: SAMPLING_ERROR, subType: ALL_SAMPLES_FILTERED_ERROR}]}
}

function simulate(expr, inputs, maxSamples) {
  const s = Evaluate(expr, maxSamples, []);
  return s
}

var Sampler = {
  sample({params: [low, high]}, n, _1) {
    // This assumes a centered 90% confidence interval, e.g. the left endpoint
    // marks 0.05% on the CDF, the right 0.95%.
    const mean = math.mean(high, low);
    const stdev = (high - mean) / 1.645;
    return simulate(`normal(${mean},${stdev})`, [], n)
  }
};

var Sampler$1 = {
  sample({params: [low, high]}, n, _1) {
    // This assumes a centered 90% confidence interval, e.g. the left endpoint
    // marks 0.05% on the CDF, the right 0.95%.
    const logHigh = math.log(high);
    const logLow = math.log(low);

    const mean = math.mean(logHigh, logLow);
    const stdev = (logHigh-logLow) / (2*1.645);

    return simulate(`lognormal(${mean},${stdev})`, [], n)
  }
};

var Sampler$2 = {
  sample({params: [hits, total]}, n, _1) {
    // This treats your entry as a prior, and assumes you are 2 times more confident than
    // a raw beta would be. This gives your distribution more of a peak for small numbers.
    return simulate(`beta(${2*hits},${2*(total-hits)})`, [], n)
  }
};

var Sampler$3 = {
  sample({params: [value]}) {
    return ({values: [value]})
  }
};

var Sampler$4 = {
  sample({params: [low, high]}, n, _1) {
    return simulate(`uniform(${low},${high})`, [], n)
  }
};

var Sampler$5 = {
  sample({text}, n, inputs) {
    return simulate(text, inputs, n)
  }
};

var Sampler$6 = {
  sample(formatted, n) {
    return Promise.resolve({values: formatted.data})
  }
};

var Sampler$7 = {
  sample(formatted) {
    return Promise.resolve({values: [], errors: []})
  }
};

const Funct = {
  referenceName: 'FUNCTION',
  types: ['FUNCTION'],
  displayName: 'Function',
  sampler: Sampler$5,
};

const DistributionNormal = {
  referenceName: 'NORMAL',
  types: ['DISTRIBUTION', 'NORMAL'],
  displayName: 'Normal',
  sampler: Sampler,
  isRangeDistribution: true,
};

const DistributionPoint = {
  referenceName: 'POINT',
  types: ['DISTRIBUTION', 'POINT'],
  displayName: 'Point',
  sampler: Sampler$3,
};

const DistributionLognormal = {
  referenceName: 'LOGNORMAL',
  types: ['DISTRIBUTION', 'LOGNORMAL'],
  displayName: 'LogNormal',
  isRangeDistribution: true,
  sampler: Sampler$1,
};

const DistributionBeta = {
  referenceName: 'BETA',
  types: ['DISTRIBUTION', 'BETA'],
  displayName: 'Beta',
  isRangeDistribution: false,
  sampler: Sampler$2,
};


const DistributionUniform = {
  referenceName: 'UNIFORM',
  types: ['DISTRIBUTION', 'UNIFORM'],
  displayName: 'Uniform',
  isRangeDistribution: true,
  sampler: Sampler$4,
};

const Data = {
  referenceName: 'DATA',
  types: ['DATA'],
  displayName: 'Data',
  isRangeDistribution: false,
  sampler: Sampler$6,
};

// Change to null Guesstimate for sampler
const None = {
  referenceName: 'NONE',
  types: [],
  displayName: 'NONE',
  sampler: Sampler$7
};

const all = [
  Funct,
  DistributionNormal,
  DistributionBeta,
  DistributionPoint,
  DistributionLognormal,
  DistributionUniform,
  Data,
  None
];

function find(referenceName) {
  const found = all.find(e => e.referenceName === referenceName);
  return found || None
}

const samplerTypes = {
  find: referenceName => find(referenceName),
  all
};

//TODO(fix this class)

class Guesstimator {
  static parse(unparsedInput) {
    const [parsedError, parsedInput] = parse(unparsedInput);
    const newItem = new this({ parsedError, parsedInput });
    return [parsedError, newItem]
  }

  constructor({ parsedError, parsedInput }) {
    this.parsedError = parsedError || {};
    this.parsedInput = parsedInput;
  }

  hasParsingErrors() {
    return !_.isEmpty(this.parsedError)
  }

  samplerType() {
    return samplerTypes.find(this.parsedInput.guesstimateType)
  }

  needsExternalInputs() {
    return (this.parsedInput.guesstimateType === 'FUNCTION')
  }

  sample(n, externalInputs = []) {
    if (!_.isEmpty(this.parsedError)) {
      return Promise.resolve({ errors: [this.parsedError], values: [] })
    }

    const samplerType = this.samplerType();
    return samplerType.sampler.sample(this.parsedInput, n, externalInputs)
  }
}

const testVar = 30;

export { Guesstimator, testVar };

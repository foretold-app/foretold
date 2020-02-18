import { Evaluate } from './simulator/evaluator.js';
import _ from 'lodash';

const WORKER_ERROR = 1;
const NO_DATA_PASSED_ERROR = 1;
const NO_EXPR_PASSED_ERROR = 1;
const NO_NUMSAMPLES_PASSED_ERROR = 1;
const ZERO_SAMPLES_REQUESTED_ERROR = 1;

onmessage = ({ data }) => {
  let errors = [];
  if (!data) {
    errors.push({
      type: WORKER_ERROR,
      subType: NO_DATA_PASSED_ERROR,
    });
    postMessage(JSON.stringify({ errors }));
    return;
  }

  data = JSON.parse(data);

  if (!data.expr) {
    errors.push({ type: WORKER_ERROR, subType: NO_EXPR_PASSED_ERROR });
  }

  if (!data.numSamples) {
    if (data.numSamples === 0) {
      errors.push({
        type: WORKER_ERROR,
        subType: ZERO_SAMPLES_REQUESTED_ERROR
      });
    } else {
      errors.push({
        type: WORKER_ERROR,
        subType: NO_NUMSAMPLES_PASSED_ERROR,
      });
    }
  }

  if (!_.isEmpty(errors)) {
    postMessage(JSON.stringify({ errors }));
  } else {
    postMessage(JSON.stringify(Evaluate(data.expr, data.numSamples, data.inputs)));
  }
};

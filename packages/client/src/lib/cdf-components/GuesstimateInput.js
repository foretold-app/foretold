import React from "react";
import _ from "lodash";
import { Input } from "antd";

import { Guesstimator } from '@foretold/guesstimator';
import { Samples, Cdf } from "../../../../cdf";

/**
 * @param {number} ratio
 * @return {string}
 */
const minMaxRatio = (ratio) => {
  if (ratio < 100000) {
    return "SMALL"
  } else if (ratio < 10000000) {
    return "MEDIUM"
  } else {
    return "LARGE"
  }
};

/**
 * @param samples
 * @return {string}
 */
const ratioSize = samples => {
  samples.sort();
  const minValue = samples.getPercentile(2);
  const maxValue = samples.getPercentile(98);
  return minMaxRatio(maxValue / minValue);
};

/**
 * @param {number[]} values
 * @param {number} min
 * @param {number} max
 * @return {[number[], number[], boolean]}
 */
const toCdf = (values, min, max) => {
  const samples = new Samples(values);
  const ratioSize$ = ratioSize(samples);
  const width = ratioSize$ === "SMALL" ? 20 : 1;

  const cdf = samples.toCdf({ size: 1000, width, min, max});
  return [cdf.ys, cdf.xs, ratioSize$ === "LARGE"];
};

export class GuesstimateInput extends React.Component {
  /**
   * @param props
   */
  constructor(props) {
    super(props);
    this.state = { value: '', items: [], inputs: {}};
    this.handleChange = this.handleChange.bind(this);
    this.textInput = React.createRef();
  }

  componentDidMount() {
    let dict = {}
    this.props.inputs.forEach(element => {
      let cdf = (new Cdf(element.xs, element.ys)).sample(10000);
      dict[element.name] = cdf;
    })
    this.setState({inputs:dict});
    if (this.props.focusOnRender) {
      this.textInput.focus();
    }
  }

  handleChange(event) {
    const text = event.target.value;

    let [_error, item] = Guesstimator.parse({ text });
    let parsedInput = item.parsedInput;
    let value = (new Guesstimator({ parsedInput: parsedInput })).sample(this.props.sampleCount, this.state.inputs);
    let values = _.filter(value.values, _.isFinite);

    if (!!values) {
      this.setState({ value: event.target.value, items: values });
    } else {
      this.setState({ value: event.target.value, items: [] });
    }

    if (!values || values.length === 0) {
      this.props.onUpdate([[], [], false]);
    } else if (values.length === 1) {
      this.props.onUpdate([[1], values, false]);
    } else {
      const min = this.props.min;
      const max = this.props.max;
      this.props.onUpdate(toCdf(values, min, max));
    }

    this.props.onChange(text);
  }

  render() {
    return <Input
      type="text"
      placeholder="10 to 100"
      value={this.state.value}
      onChange={this.handleChange}
      ref={input => this.textInput = input}
    />;
  }
}
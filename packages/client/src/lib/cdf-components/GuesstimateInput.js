import React from "react";
import _ from "lodash";
import { Input } from "antd";

import { Guesstimator } from '@foretold/guesstimator';
import { Samples, Cdf } from "../../../../cdf";

/**
 *
 * @param {number} minValue
 * @param {number} maxValue
 * @returns {string}
 */
const minMaxRatio = (minValue, maxValue) => {
  if (minValue === 0 || maxValue === 0) {
    return "SMALL"
  }
  const ratio = maxValue / minValue;
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
  return minMaxRatio(minValue, maxValue);
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

  const cdf = samples.toCdf({ size: 1000, width, min, max });
  return [cdf.ys, cdf.xs, ratioSize$ === "LARGE"];
};

export class GuesstimateInput extends React.Component {
  /**
   * @param props
   * @param props.onUpdate
   * @param props.onChange
   * @param {{name: string, xs: number[], ys: number[]}[]} props.inputs
   * @param props.focusOnRender
   * @param {number} props.sampleCount
   * @param {number} props.min
   * @param {number} props.max
   */
  constructor(props) {
    super(props);
    this.state = {
      value: '',
      items: [],
      inputs: {},
    };
    this.textInput = React.createRef();
  }

  componentDidMount() {
    const dict = {};
    this.props.inputs.forEach(element => {
      dict[element.name] = new Cdf(element.xs, element.ys).sample(10000);
    });
    this.setState({ inputs: dict });

    if (this.props.focusOnRender) {
      this.textInput.focus();
    }
  }

  /**
   * @param event
   */
  handleChange(event) {
    const text = event.target.value;

    let [_error, item] = Guesstimator.parse({ text });

    const parsedInput = item.parsedInput;
    const value = new Guesstimator({ parsedInput }).sample(
      this.props.sampleCount,
      this.state.inputs,
    );

    const values = _.filter(value.values, _.isFinite);

    this.setState({
      value: event.target.value,
      items: values,
    });

    if (values.length === 0) {
      this.props.onUpdate([[], [], false]);
    } else if (values.length === 1) {
      this.props.onUpdate([[1], values, false]);
    } else {
      const { min, max } = this.props;
      this.props.onUpdate(toCdf(values, min, max));
    }

    this.props.onChange(text);
  }

  render() {
    return <Input
      type="text"
      placeholder="10 to 100"
      value={this.state.value}
      onChange={event => this.handleChange(event)}
      ref={input => this.textInput = input}
    />;
  }
}
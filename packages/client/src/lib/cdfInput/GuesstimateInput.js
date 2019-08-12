import React from "react";
import { Guesstimator } from '@foretold/guesstimator/index';
import { Input } from "antd";
import { Samples } from "../../../../cdf";
import _ from "lodash"

/**
 * @param ratio
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
let ratioSize = samples => {
  samples.sort();
  const minValue = samples.getPercentile(2);
  const maxValue = samples.getPercentile(98);
  return minMaxRatio(maxValue / minValue);
};

/**
 * @param values
 * @param min
 * @param max
 * @return {*[]}
 */
const toCdf = (values, min, max) => {
  let _values = values;
  if (_.isFinite(min)) _values = _.filter(_values, r => r > min);
  if (_.isFinite(max)) _values = _.filter(_values, r => r < max);

  console.log("values toCdf", { _values });
  const samples = new Samples(_values);
  console.log("toCdf::samples", { samples });

  const ratioSize$ = ratioSize(samples);
  const width = ratioSize$ === "SMALL" ? 20 : 1;
  console.log("toCdf::width", { width });

  const cdf = samples.toCdf({ size: 1000, width, min, max });
  console.log("toCdf::toCdf",{ size: 1000, width, min, max });
  console.log("toCdf::cdf", { cdf });

  return [cdf.ys, cdf.xs, ratioSize$ === "LARGE"];
};

export class GuesstimateInput extends React.Component {
  /**
   * @param props
   */
  constructor(props) {
    super(props);
    this.state = { value: '', items: [] };
    this.handleChange = this.handleChange.bind(this);
    this.textInput = React.createRef();
  }

  componentDidMount() {
    if (this.props.focusOnRender) {
      this.textInput.focus();
    }
  }

  handleChange(event) {
    const text = event.target.value;

    let [error, item] = Guesstimator.parse({ text });
    console.log("A", { error, item });
    let parsedInput = item.parsedInput;
    console.log("B", { parsedInput });
    let what = new Guesstimator({ parsedInput: parsedInput });
    console.log("c", { what });
    let foo = what.sample(this.props.sampleCount);
    console.log("D", { foo });
    let values = _.filter(foo.values, _.isFinite);
    console.log("E", { values });

    if (!!values) {
      this.setState({ value: event.target.value, items: values });
    } else {
      this.setState({ value: event.target.value, items: [] });
    }

    if (!values) {
      this.props.onUpdate([[], [], false]);
    } else if (values.length === 1) {
      this.props.onUpdate([[1], values, false]);
    } else {
      this.props.onUpdate(toCdf(values, this.props.min, this.props.max));
    }
    this.props.onChange(text);
  }

  render() {
    return (<Input type="text" placeholder="10 to 100" value={this.state.value}
                   onChange={this.handleChange}
                   ref={input => this.textInput = input}/>
    )
  }
}
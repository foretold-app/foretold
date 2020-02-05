import AutosizeInput from 'react-input-autosize';
import React from 'react';

export class AutosizeTextareaInput extends React.Component {
  /**
   * @param props
   */
  constructor(props) {
    super(props);
    this.state = { value: this.props.value || "" };
    this.handleChange = this.handleChange.bind(this);
  }

  handleChange(event) {
    const { value } = event.target;
    this.setState({ value });
    this.props.onChange(value);
  }

  render() {
    return React.createElement(AutosizeInput, {
      name: this.props.name,
      placeholder: this.props.placeholder,
      inputClassName: this.props.inputClassName,
      injectStyles: false,
      value: this.state.value,
      style: this.props.style,
      inputStyle: { transition: "none" },
      onChange: this.handleChange,
    });
  }
}

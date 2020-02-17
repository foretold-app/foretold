import { Vega } from 'react-vega';
import React from 'react';
import spec from './spec-percentiles';

export class PercentilesChart extends React.Component {
  /**
   * @param props
   */
  constructor(props) {
    super(props);
    this.handleHover = this.handleHover.bind(this);
    this.props = {
      data: {},
      spec: {}
    }
  }


  handleHover() {

  }

  render() {
    return React.createElement(Vega, {
      spec: spec,
      onSignalHover: this.handleHover,
    });
  }
}

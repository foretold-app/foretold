import React from 'react';
import * as vega from 'vega';

import spec from "./spec-percentiles";

export class PercentilesChart extends React.PureComponent {

  constructor(props) {
    super(props);
    this.containerRef = React.createRef();
    this.view = null;
  }

  componentDidMount() {
    this.view = new vega.View(vega.parse(spec), {
      renderer: 'canvas',
      container: this.containerRef.current,
      hover: true
    });
    return this.view.runAsync();
  }

  componentWillUnmount() {
    if (this.view) {
      this.view.finalize();
    }
  }

  render() {
    return React.createElement("div", {
      ref: this.containerRef,
    });
  }
}
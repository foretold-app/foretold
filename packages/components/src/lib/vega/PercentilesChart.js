import * as _ from 'lodash';
import React from 'react';
import * as vega from 'vega';
import { Samples } from '@foretold/cdf';

import spec from './spec-percentiles';

export class PercentilesChart extends React.PureComponent {

  constructor(props) {
    super(props);
    this.containerRef = React.createRef();
    this.view = null;

    this.data = this.props.data.map(item => {
      const samples = new Samples(item.ys);
      samples.sort();
      const p5 = samples.getPercentile(5);
      const p50 = samples.getPercentile(50);
      const p95 = samples.getPercentile(95);
      item.p5 = p5;
      item.p50 = p50;
      item.p95 = p95;
      const timestamp = item.createdAt;
      return { p5, p50, p95, timestamp };
    });

    this.spec = _.cloneDeep(spec);
    this.spec.data[0].values = this.data;
  }

  componentDidMount() {
    this.view = new vega.View(vega.parse(this.spec), {
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
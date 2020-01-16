import React from 'react';
import chart from "./cdfChartd3"

/**
 * @param min
 * @param max
 * @returns {number}
 */
function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

/**
 * Example input:
 * {
 * xs: [50,100,300,400,500,600],
 * ys: [0.1, 0.4, 0.6, 0.7,0.8, 0.9]}
 * }
 */
class CdfChart extends React.Component {
  constructor(props){
    super(props);
    this.state = {
      divId: "chart-" + getRandomInt(0,100000)
    }
  }

  componentDidMount() {
    this.drawChart();
  }

  componentDidUpdate() {
    this.drawChart();
  }

  /**
   * TODO: Fix for log when minX is 0;
   */
  drawChart() {
    const _chart = chart()
      .svgHeight(this.props.height)
      .maxX(this.props.maxX)
      .minX(this.props.minX)
      .marginBottom(this.props.marginBottom || 15)
      .marginLeft(5)
      .marginRight(5)
      .showDistributionLines(this.props.showDistributionLines)
      .marginTop(5)
      .verticalLine(this.props.verticalLine)
      .showVerticalLine(this.props.showVerticalLine)
      .onHover(e => {})
      .container("#" + this.state.divId);
    _chart.data({primary: this.props.primaryDistribution}).render();
  }

  render() {
    return React.createElement("div", {
      id: this.state.divId,
      style: this.props.width ? { width: this.props.width + "px" } : {}
    });
  }
}

export default CdfChart;
import React from 'react';
import chart from "./cdfChartd3"
import _ from "lodash"

function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

// Example input: {xs: [50,100,300,400,500,600], ys: [0.1, 0.4, 0.6, 0.7,0.8, 0.9]}}
class CdfChart extends React.Component {
  constructor(props){
    super(props)
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
    
  drawChart() {
    // TODO: Fix for log when minX is 0;
    var _chart = chart()
          .svgHeight(this.props.height)
          .maxX(this.props.maxX)
          .marginBottom(_.isFinite(this.props.marginBottom) ?  this.props.marginBottom : 15)
          .marginLeft(5)
          .marginRight(5)
          .showDistributionLines(this.props.showDistributionLines)
          .marginTop(_.isFinite(this.props.marginTop) ? this.props.marginTop : 10)
          .verticalLine(this.props.verticalLine)
          .showVerticalLine(this.props.showVerticalLine)
          .onHover(e => this.props.onHover(e))
          .container("#" + this.state.divId);
    _chart.data({primary: this.props.primaryDistribution}).render();
  }
        
  render(){
    return <div id={this.state.divId} style={{width: this.props.width + "px"}}/>
  }
}

export default CdfChart;
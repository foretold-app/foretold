import React from 'react';
import chart from "./cdfChartd3"

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
          .marginBottom(this.props.marginBottom || 15)
          .marginLeft(0)
          .marginRight(0)
          .showDistributionLines(this.props.showDistributionLines)
          .marginTop(5)
          .verticalLine(this.props.verticalLine)
          .showVerticalLine(this.props.showVerticalLine)
          .onHover(e => {})
          .container("#" + this.state.divId);
    _chart.data({primary: this.props.primaryDistribution}).render();
  }
        
  render(){
    return <div id={this.state.divId} style={{width: this.props.width + "px"}}/>
  }
}

export default CdfChart;
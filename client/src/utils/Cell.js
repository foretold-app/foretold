import React from "react";
import {Guesstimator} from '../lib/guesstimator/index';
import {VictoryChart, VictoryLine} from "victory";
import _ from "lodash";

const cdf = (values) => {
    const sorted = _.sortBy(values)
    const length = values.length
    return _.map(sorted, (o,i) => ({x: o, y: i / length}))
}

const pdf = (values, chunkSize) => {
    let _cdf = cdf(values);
    let inChunks = _cdf.filter(function(value, index, Arr) {
        return index % chunkSize == 0;
    });
    return _.map(inChunks, (o,i) => {
        if (i == 0 ){
            return ({x: o.x, y: 0})
        } else {
            let derivative = (o.y - (_cdf[i - 1].y)) / (o.x - (_cdf[i - 1].x))
            return ({x: o.x, y: derivative})
        }
    })
}

const ccdf = (values) => {
    const sorted = _.sortBy(values)
    const length = values.length
    return [sorted, _.map(sorted, (o,i) => (i / length))]
}

export class Cell extends React.Component {
    constructor(props) {
        super(props);
        this.state = {value: '', items:[]};
        this.handleChange = this.handleChange.bind(this);
    }
    handleChange(event) {
        let [error, item] = Guesstimator.parse({text: event.target.value})
        let parsedInput = item.parsedInput;
        let what = new Guesstimator({parsedInput: parsedInput})
        let foo = what.sample(this.props.sampleCount)
        let values = foo.values;
        if (!!values) {
            this.setState({value: event.target.value, items: values});
        } else {
            this.setState({value: event.target.value, items: []});
        }
        this.props.onUpdate(!!values ? ccdf(values): [])
      }
    
    chunkSize(){
        return this.props.sampleCount / this.props.cdfCount
    }
    

    render() {
        return (<div><input type="text" value={this.state.value} onChange={this.handleChange}/>
        <VictoryChart
>
  <VictoryLine
    style={{
      data: { stroke: "#888" },
      parent: { border: "1px solid #ccc"}
    }}
    data={pdf(this.state.items, this.chunkSize())}
  />
</VictoryChart>
        </div>)
    }
}
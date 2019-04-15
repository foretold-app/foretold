import React from "react";
import {Guesstimator} from '../guesstimator/index';
import {VictoryChart, VictoryLine} from "victory";
import {Input} from "antd";
import _ from "lodash";

const toCdf = (values) => {
    const sorted = _.sortBy(values)
    const length = values.length
    return [_.map(sorted, (o,i) => (i / length)), sorted]
}

export class GuesstimateInput extends React.Component {
    constructor(props) {
        super(props);
        this.state = {value: '', items:[]};
        this.handleChange = this.handleChange.bind(this);
        this.textInput = React.createRef();
    }
    componentDidMount(){
        if (this.props.focusOnRender){
            this.textInput.focus();
        }
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
        this.props.onUpdate(!!values ? toCdf(values): [[], []])
      }
    
    render() {
        return (<Input type="text" value={this.state.value} onChange={this.handleChange} ref={input => this.textInput = input}/>
        )
    }
}
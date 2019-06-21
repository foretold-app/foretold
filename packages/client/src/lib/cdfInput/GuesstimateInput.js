import React from "react";
import {Guesstimator} from '../guesstimator/index';
import {Input} from "antd";
import {Samples} from "@foretold/cdf";

const toCdf = (values) => {
    const samples = new Samples(values);
    const cdf = samples.toCdf({size:1000, width:20});
    return [cdf.ys, cdf.xs];
};

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
        let [error, item] = Guesstimator.parse({text: event.target.value});
        let parsedInput = item.parsedInput;
        let what = new Guesstimator({parsedInput: parsedInput});
        let foo = what.sample(this.props.sampleCount);
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
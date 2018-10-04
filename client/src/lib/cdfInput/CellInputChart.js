import React from "react";
import {
    VictoryChart,
    VictoryLine
} from "victory";
import _ from "lodash";

const pdf = (values, chunkSize) => {
    let _cdf = (values);
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

export class CellInputChart extends React.Component {
        render() {
            return ( <
                VictoryChart >
                <
                VictoryLine
                    data = {
                        pdf(this.props.data, 10)
                    }
                />
                </VictoryChart>
            )
        }
}
import React from "react";
import {
    VictoryChart,
    VictoryAxis,
    VictoryLine
} from "victory";
import _ from "lodash";

const pdf = (values, chunkSize) => {
    let _cdf = (values);
    let inChunks = _cdf.filter(function(value, index, Arr) {
        return index % chunkSize == 0;
    });
    let items = _.map(inChunks, (o,i) => {
        if (i == 0 ){
            return ({x: o.x, y: 0})
        } else {
            let derivative = (o.y - (inChunks[i - 1].y)) / (o.x - (inChunks[i - 1].x))
            console.log(o, inChunks[i-1], (o.y - (inChunks[i - 1].y)), (o.x - (inChunks[i - 1].x)), derivative)
            return ({x: o.x, y: derivative})
        }
    })
    items.splice(1, -1)
    return items
}

export class CellInputChart extends React.Component {
        render() {
            let {data} = this.props;
            let pdff = pdf(data, 100);
            let getE = (fn, pdff, p) => {
                const item = fn(pdff, r => p(r));
                return item && p(item)
            }
            return ( <
                VictoryChart
                    padding={{top: 1, bottom: 9, right: 1, left: 3}}
                    height={40}
                    width={150}
                    maxDomain={{"y": getE(_.maxBy, pdff, (e => e.y)) * 1.1, "x": getE(_.maxBy, pdff, (e => e.x)) * 1.1}}
                    minDomain={{"y": getE(_.minBy, pdff, (e => e.y)), "x": getE(_.minBy, pdff, (e => e.x))}}
                >
                <VictoryAxis
                height={200}
                style={{
                    axis: {stroke: "#aaa"},
                    tickLabels: {fontSize: 7, padding: 2}
                }}
                />
                <
                VictoryLine
                    data={pdff}
                    style={{data: {stroke: "#aaa"}}}
                />
                </VictoryChart>
            )
        }
}

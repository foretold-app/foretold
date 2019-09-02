import React from "react";
import { TimeSeries } from "pondjs";
import {ChartContainer, ChartRow, YAxis, Charts, LineChart, Baseline} from "react-timeseries-charts";

const points =[[1416787200000,-2.2],[1416182400000,-2.1],[1415577600000,-1.5],[1414972800000,1.3],[1414368000000,0.790814285714286]].reverse();

const series = new TimeSeries({
    name: "USD_vs_EURO",
    columns: ["time", "value"],
    points
});

const style = {
    value: {
        stroke: "#a02c2c",
        opacity: 0.2
    }
};

const baselineStyle = {
    line: {
        stroke: "steelblue",
        strokeWidth: 1,
        opacity: 0.4,
        strokeDasharray: "none"
    },
    label: {
        fill: "steelblue"
    }
};

const baselineStyleLite = {
    line: {
        stroke: "steelblue",
        strokeWidth: 1,
        opacity: 0.5
    },
    label: {
        fill: "steelblue"
    }
};

const baselineStyleExtraLite = {
    line: {
        stroke: "steelblue",
        strokeWidth: 1,
        opacity: 0.2,
        strokeDasharray: "1,1"
    },
    label: {
        fill: "steelblue"
    }
};

export class TimeSeriesChart extends React.Component {
  render() {
    return(
<ChartContainer timeRange={series.range()} format="%d %b">
        <ChartRow height="150">
            <YAxis
                id="price"
                label="Price ($)"
                min={series.min()} max={series.max()}
                width="60" format=".2f"/>
            <Charts>
                <LineChart axis="price" series={series}/>
            </Charts>
        </ChartRow>
    </ChartContainer>
    )}
}
import React from "react";
import { TimeSeries } from "pondjs";
import {ChartContainer, ChartRow, YAxis, Charts, LineChart, Baseline, EventMarker} from "react-timeseries-charts";

const points =[[1416787200000,-2.2],[1416182400000,-3.1],[1415577600000,-1.5],[1415575600000,-1.5],[1414972800000,1.3],[1414572800000,2],[1414368000000,-2]].reverse();

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
    constructor(props){
        super(props);
        this.state = {
            tracker: null,
            trackerValue: "-- °C",
            trackerEvent: null,
            markerMode: "flag"
        };
    };

    handleTrackerChanged(t) {
        if (t) {
            const e = series.atTime(t);
            const eventTime = new Date(
                e.begin().getTime() + (e.end().getTime() - e.begin().getTime()) / 2
            );
            const eventValue = e.get("value");
            const v = `yo dog ${eventValue}`;
            this.setState({ tracker: eventTime, trackerValue: v, trackerEvent: e });
        } else {
            this.setState({ tracker: null, trackerValue: null, trackerEvent: null });
        }
    };

  render() {
    return(
    <ChartContainer timeRange={series.range()} format="%d %b" onTrackerChanged={this.handleTrackerChanged.bind(this)}>
        <ChartRow height="150">
            <YAxis
                id="axis"
                transition={300}
                min={-3.5}
                max={3.5}
                width="60"
                format=",.1f"
                type="linear"
            />
            <Charts>
                <LineChart axis="axis" series={series}/>
                <EventMarker
                    type="flag"
                    axis="axis"
                    event={this.state.trackerEvent}
                    info={[{label: "Anamaly", value: this.state.trackerValue}]}
                    infoTimeFormat="%d %b"
                    infoWidth={120}
                    markerRadius={5}
                    markerStyle={{ fill: "black" }}
                />
            </Charts>
        </ChartRow>
    </ChartContainer>
    )}
}
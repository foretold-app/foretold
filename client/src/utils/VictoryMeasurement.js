import React from "react";

export class VictoryMeasurement extends React.Component {
    render() {
        const {
            scale
        } = this.props;
        const {
            x,
            y1,
            y2,
            y3
        } = this.props.point;
        const xx = scale.x(x);
        return ( <
            svg 
            >
            <
            line x1 = {
                xx
            }
            y1 = {
                scale.y(y1)
            }
            x2 = {
                xx
            }
            y2 = {
                scale.y(y3)
            }
            stroke = "#bbb" / >
            <
            circle cx = {
                xx
            }
            cy = {
                scale.y(y1)
            }
            r = "2"
            x = "10"
            fill = "#888" / >
            <
            circle cx = {
                xx
            }
            cy = {
                scale.y(y2)
            }
            r = "2"
            x = "10"
            fill = "#111" / >
            <
            circle cx = {
                xx
            }
            cy = {
                scale.y(y3)
            }
            r = "2"
            x = "10"
            fill = "#888" / >
            <
            /svg>
        );
    }
}
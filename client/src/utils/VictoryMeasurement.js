import React from "react";

const colors = {
    GRAY: {
        light: "#bbb",
        medium: "#888",
        dark: "#111",
        r: "2"
    }, 
    BLUE: {
        light: "#94b9e6",
        medium: "#489ece",
        dark: "#007fc5",
        r: "3"
    } 
}

export class VictoryMeasurement extends React.Component {
    render() {
        const {
            scale,
            color
        } = this.props;
        const {
            x,
            y1,
            y2,
            y3
        } = this.props.point;
        const xx = scale.x(x);
        const col = colors[color];
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
            stroke = {col.light} / >
            <
            circle cx = {
                xx
            }
            cy = {
                scale.y(y1)
            }
            r = {col.r}
            x = "10"
            fill = {col.medium} / >
            <
            circle cx = {
                xx
            }
            cy = {
                scale.y(y2)
            }
            r = {col.r}
            x = "10"
            fill = {col.dark} / >
            <
            circle cx = {
                xx
            }
            cy = {
                scale.y(y3)
            }
            r = {col.r}
            x = "10"
            fill = {col.medium} / >
            <
            /svg>
        );
    }
}
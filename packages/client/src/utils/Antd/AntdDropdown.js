import React from "react";
import { Dropdown as Drop } from 'antd';

export class Dropdown extends React.Component {
  render() {
    return (
      <Drop
        trigger="click"
        overlayClassName={this.props.overlayClassName}
        overlay={<div>{this.props.overlay}</div>}
        placement="bottomRight"
      >
        <a className="ant-dropdown-link" href="#">
          {this.props.children}
        </a>
      </Drop>
    )
  }
}
import React from "react";
import { Menu, Dropdown as Drop, Icon } from 'antd';

const menu = (
    <Menu>
      <Menu.Item>
        <a target="_blank" rel="noopener noreferrer" href="http://www.alipay.com/">1st menu item</a>
      </Menu.Item>
      <Menu.Item>
        <a target="_blank" rel="noopener noreferrer" href="http://www.taobao.com/">2nd menu item</a>
      </Menu.Item>
      <Menu.Item>
        <a target="_blank" rel="noopener noreferrer" href="http://www.tmall.com/">3rd menu item</a>
      </Menu.Item>
    </Menu>
  );

export class Dropdown extends React.Component {
    render() {
        return(
            <Drop overlay={<div>{this.props.overlay}</div>}>
            <a className="ant-dropdown-link" href="#">
            {this.props.children}
            </a>
          </Drop>
        )
    }
}
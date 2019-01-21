import React from "react";
import { Icon as Iconn } from 'react-icons-kit'
import { home } from 'react-icons-kit/icomoon/home'
import {user} from 'react-icons-kit/ikons/user'


let types = {
    "HOME": home,
    "USER":user 
}

export class Icon extends React.Component {
    render() {
        const {
            iconType,
            size
        } = this.props;
        return ( <Iconn size={size} icon={types[iconType]} />)
    }
}
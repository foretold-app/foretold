import React from "react";
import { Icon as Iconn } from 'react-icons-kit'
import { home } from 'react-icons-kit/icomoon/home'
import {user} from 'react-icons-kit/ikons/user'
import {arrowLeft} from 'react-icons-kit/fa/arrowLeft'
import {arrowRight} from 'react-icons-kit/fa/arrowRight'
import {columns} from 'react-icons-kit/ikons/columns'
import {layers} from 'react-icons-kit/feather/layers'
import {ic_people} from 'react-icons-kit/md/ic_people'
import {bulb} from 'react-icons-kit/entypo/bulb'
import {socialBuffer} from 'react-icons-kit/ionicons/socialBuffer'
import {flag} from 'react-icons-kit/entypo/flag'
import {cap} from 'react-icons-kit/entypo/cap'
import {flash} from 'react-icons-kit/entypo/flash'
import {gavel} from 'react-icons-kit/fa/gavel'


let types = {
    "HOME": home,
    "USER":user,
    "COLUMNS":columns,
    "LAYERS":socialBuffer,
    "PEOPLE": ic_people,
    "FLASH": flash,
    "GAVEL": gavel,
    "BULB": bulb,
    "ARROW_RIGHT": arrowRight,
    "ARROW_LEFT": arrowLeft
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
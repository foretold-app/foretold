import React from "react";
import { Icon as Iconn } from 'react-icons-kit'
import { home } from 'react-icons-kit/icomoon/home'
import {user} from 'react-icons-kit/ikons/user'
import {arrowLeft2} from 'react-icons-kit/icomoon/arrowLeft2'
import {arrowRight2} from 'react-icons-kit/icomoon/arrowRight2'
import {columns} from 'react-icons-kit/ikons/columns'
import {layers} from 'react-icons-kit/feather/layers'
import {ic_people} from 'react-icons-kit/md/ic_people'
import {bulb} from 'react-icons-kit/entypo/bulb'
import {socialBuffer} from 'react-icons-kit/ionicons/socialBuffer'
import {flag} from 'react-icons-kit/entypo/flag'
import {cap} from 'react-icons-kit/entypo/cap'
import {flash} from 'react-icons-kit/entypo/flash'
import {gavel} from 'react-icons-kit/fa/gavel'
import {plus as circlePlus} from 'react-icons-kit/metrize/plus'
import {thinRight} from 'react-icons-kit/entypo/thinRight'
import {lock} from 'react-icons-kit/icomoon/lock'
import {chevronLeft} from 'react-icons-kit/fa/chevronLeft'
import {chevronRight} from 'react-icons-kit/fa/chevronRight'
import {chevronDown} from 'react-icons-kit/fa/chevronDown'


let types = {
    "HOME": home,
    "LOCK": lock,
    "USER":user,
    "COLUMNS":columns,
    "LAYERS":socialBuffer,
    "PEOPLE": ic_people,
    "FLASH": flash,
    "GAVEL": gavel,
    "BULB": bulb,
    "CIRCLE_PLUS": circlePlus,
    "ARROW_RIGHT": arrowRight2,
    "ARROW_LEFT": arrowLeft2,
    "CHEVRON_LEFT": chevronLeft,
    "CHEVRON_RIGHT": chevronRight,
    "CHEVRON_DOWN": chevronDown,
    "THIN_RIGHT":thinRight 
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
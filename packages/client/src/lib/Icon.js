import React from 'react';
import { Icon as Iconn } from 'react-icons-kit';
import { home } from 'react-icons-kit/typicons/home';
import { user } from 'react-icons-kit/ikons/user';
import { arrowLeft2 } from 'react-icons-kit/icomoon/arrowLeft2';
import { arrowRight2 } from 'react-icons-kit/icomoon/arrowRight2';
import { earth } from 'react-icons-kit/icomoon/earth';
import { columns } from 'react-icons-kit/ikons/columns';
import { ic_people } from 'react-icons-kit/md/ic_people';
import { bulb } from 'react-icons-kit/entypo/bulb';
import { socialBuffer } from 'react-icons-kit/ionicons/socialBuffer';
import { flash } from 'react-icons-kit/entypo/flash';
import { gavel } from 'react-icons-kit/fa/gavel';
import { plus as circlePlus } from 'react-icons-kit/metrize/plus';
import { thinRight } from 'react-icons-kit/entypo/thinRight';
import { thinLeft } from 'react-icons-kit/entypo/thinLeft';
import { lock } from 'react-icons-kit/icomoon/lock';
import { chevronDown } from 'react-icons-kit/fa/chevronDown';
import { emailUnread } from 'react-icons-kit/ionicons/emailUnread';
import { u26FA as tent } from 'react-icons-kit/noto_emoji_regular/u26FA';
import { ic_content_copy } from 'react-icons-kit/md/ic_content_copy';
import { magicWand } from 'react-icons-kit/icomoon/magicWand';
import { pacman } from 'react-icons-kit/icomoon/pacman';
import { thList as list } from 'react-icons-kit/typicons/thList';

let types = {
  'HOME': home,
  'LOCK': lock,
  'USER': user,
  'COLUMNS': columns,
  'EARTH': earth,
  'TENT': tent,
  'LAYERS': socialBuffer,
  'PEOPLE': ic_people,
  'FLASH': flash,
  'GAVEL': gavel,
  'BULB': bulb,
  'CIRCLE_PLUS': circlePlus,
  'ARROW_RIGHT': arrowRight2,
  'ARROW_LEFT': arrowLeft2,
  'CHEVRON_LEFT': thinLeft,
  'CHEVRON_RIGHT': thinRight,
  'CHEVRON_DOWN': chevronDown,
  'THIN_RIGHT': thinRight,
  'EMAIL_UNREAD': emailUnread,
  'COPY': ic_content_copy,
  'MAGIC_WAND': magicWand,
  'PACMAN': pacman,
  'LIST': list,
};

export class Icon extends React.Component {
  render() {
    const {
      iconType,
      size
    } = this.props;
    return (<Iconn size={size} icon={types[iconType]}/>);
  }
}

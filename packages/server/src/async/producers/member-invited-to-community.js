const { Producer } = require('./producer');
const { MemberToCommunity } = require('./member-to-community');

class MemberInvitedToCommunity extends MemberToCommunity {
  constructor(invitation = {}) {
    super(invitation);
    this.templateName = Producer.TEMPLATE_NAME.MEMBER_INVITED_TO_COMMUNITY;
  }
}

module.exports = {
  MemberInvitedToCommunity,
};

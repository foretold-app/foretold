module.exports.measurableEmptyName = () => 'Question should have either name'
  + ' or subject.';
module.exports.measurableNameLess3 = () => 'Question name should '
  + 'be more than 3 chars.';
module.exports.measurableLabelSubjectLess3 = () => 'Question subject label '
  + 'should be more than 3 chars.';
module.exports.measurableLabelPropertyLess3 = () => 'Question property label '
  + 'should be more than 3 chars.';
module.exports.measurableIsNotOpen = () => 'Measurable should be in an Open'
  + ' state.';
module.exports.measurableValueType = () => 'Measurable value type is required.';
module.exports.measurementValueTypeWrong = () => 'Measurement'
  + ' type is denied for the type of measurable.';
module.exports.channelDoesNotExists = () => 'A channel is required.';
module.exports.agentDoesNotExists = () => 'An agent is required.';
module.exports.notebookDoesNotExists = () => 'A notebook is required.';
module.exports.agentIsNotInAChannel = () => 'An agent is not in a channel.';

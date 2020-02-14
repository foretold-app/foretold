/* Untyped file generated from Graph_T.re by genType. */

const $$toRE919876083 = {"Item": 0};

import * as CreateBucklescriptBlock from 'bs-platform/lib/es6/block.js';

import * as Graph_TBS from './Graph_T.bs';

export const Thing_to_s = function (Arg1) {
  const result = Graph_TBS.Thing.to_s({thingId:Arg1.thingId, thingType:typeof(Arg1.thingType) === 'object'
    ? CreateBucklescriptBlock.__(0, [{thingIdString:Arg1.thingType.thingIdString, subjectId:Arg1.thingType.subjectId, propertyId:Arg1.thingType.propertyId, value:{valueType:Arg1.thingType.value.valueType.tag==="String"
      ? CreateBucklescriptBlock.__(0, [Arg1.thingType.value.valueType.value])
      : Arg1.thingType.value.valueType.tag==="ThingId"
      ? CreateBucklescriptBlock.__(1, [Arg1.thingType.value.valueType.value])
      : CreateBucklescriptBlock.__(2, [Arg1.thingType.value.valueType.value])}}])
    : $$toRE919876083[Arg1.thingType]});
  return result
};

export const Thing_to_json = function (Arg1) {
  const result = Graph_TBS.Thing.to_json({thingId:Arg1.thingId, thingType:typeof(Arg1.thingType) === 'object'
    ? CreateBucklescriptBlock.__(0, [{thingIdString:Arg1.thingType.thingIdString, subjectId:Arg1.thingType.subjectId, propertyId:Arg1.thingType.propertyId, value:{valueType:Arg1.thingType.value.valueType.tag==="String"
      ? CreateBucklescriptBlock.__(0, [Arg1.thingType.value.valueType.value])
      : Arg1.thingType.value.valueType.tag==="ThingId"
      ? CreateBucklescriptBlock.__(1, [Arg1.thingType.value.valueType.value])
      : CreateBucklescriptBlock.__(2, [Arg1.thingType.value.valueType.value])}}])
    : $$toRE919876083[Arg1.thingType]});
  return result
};

export const Thing = Graph_TBS.Thing

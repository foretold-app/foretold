module JsonKeys = {
  let facts = "facts";
  let things = "things";
  let thingIdField = "id";
};

module FactJson = {
  module Value = {
    let dataTypeField = "dataValue";
    let dataField = "data";
    let jsonType = "json";
    let stringType = "string";
    let thingIdType = "thingId";
  };
  module Fields = {
    let id = "id";
    let subjectId = "subjectId";
    let propertyId = "propertyId";
    let value = "value";
    let baseId = "baseId";
    let resourceId = "resourceId";
    let idIsPublic = "idIsPublic";
  };
};
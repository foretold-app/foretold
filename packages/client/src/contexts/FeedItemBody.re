type generic = {
  item: string,
  description: string,
};

type measurable = {
  item: string,
  description: string,
  measurableId: string,
};

type t = {
  generic: option(generic),
  measurable: option(measurable),
};

let make = (~generic, ~measurable, ()): t => {generic, measurable};

module Generic = {
  type t = generic;
  let make = (~item, ~description, ()): t => {item, description};
};

module Measurable = {
  type t = measurable;
  let make = (~item, ~description, ~measurableId, ()): t => {
    item,
    description,
    measurableId,
  };
};
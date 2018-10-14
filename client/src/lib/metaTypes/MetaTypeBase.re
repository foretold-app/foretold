open Utils;

type metaTypes = [ | `Date | `Entity | `String];

/* The ID of the entity */
type entity = string;

type human = {name: string};

type measurable = {id: string};

type action('a) = {
  name: string,
  metaTypes: array((string, metaTypes)),
};

module type MeasurableTypeModule = {
  type t;
  let name: string;
  let id: string;
  let description: string;
  let metaTypes: array((string, metaTypes));
};

type measurableType = {
  name: string,
  id: string,
  description: string,
  metaTypes: array((string, metaTypes)),
  form: ReasonReact.reactElement,
};

module MakeMeasurableType = (Item: MeasurableTypeModule) => {
  let make = {
    id: Item.id,
    name: Item.name,
    description: Item.description,
    metaTypes: Item.metaTypes,
    form:
      <MetaTypeForm
        key=Item.id
        fields=(Array.map(((a, b)) => a, Item.metaTypes))
      />,
  };
};